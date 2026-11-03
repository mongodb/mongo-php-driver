/*
 * Copyright 2024-present MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Note: this file was derived from libbson's bson-atomic.c */

#include "bson/bson.h"

#include "phongo_atomic.h"

#ifdef BSON_OS_UNIX
/* For sched_yield() */
#include <sched.h>
#endif

static void _thrd_yield(void)
{
	BSON_IF_WINDOWS(SwitchToThread();)
	BSON_IF_POSIX(sched_yield();)
}

/*
 * Some platforms do not support compiler intrinsics for atomic operations.
 * We emulate that here using a spin lock and regular arithmetic operations
 */
static int8_t gEmulAtomicLock = 0;

static void _lock_emul_atomic(void)
{
	int i;
	if (phongo_atomic_int8_compare_exchange_weak(&gEmulAtomicLock, 0, 1, phongo_memory_order_acquire) == 0) {
		/* Successfully took the spinlock */
		return;
	}
	/* Failed. Try taking ten more times, then begin sleeping. */
	for (i = 0; i < 10; ++i) {
		if (phongo_atomic_int8_compare_exchange_weak(&gEmulAtomicLock, 0, 1, phongo_memory_order_acquire) == 0) {
			/* Succeeded in taking the lock */
			return;
		}
	}
	/* Still don't have the lock. Spin and yield */
	while (phongo_atomic_int8_compare_exchange_weak(&gEmulAtomicLock, 0, 1, phongo_memory_order_acquire) != 0) {
		_thrd_yield();
	}
}

static void _unlock_emul_atomic(void)
{
	int64_t rv = phongo_atomic_int8_exchange(&gEmulAtomicLock, 0, phongo_memory_order_release);
	BSON_ASSERT(rv == 1 && "Released atomic lock while not holding it");
}

int32_t _phongo_emul_atomic_int32_fetch_add(volatile int32_t* p, int32_t n, enum phongo_memory_order _unused)
{
	int32_t ret;

	BSON_UNUSED(_unused);

	_lock_emul_atomic();
	ret = *p;
	*p += n;
	_unlock_emul_atomic();
	return ret;
}

int32_t _phongo_emul_atomic_int32_exchange(volatile int32_t* p, int32_t n, enum phongo_memory_order _unused)
{
	int32_t ret;

	BSON_UNUSED(_unused);

	_lock_emul_atomic();
	ret = *p;
	*p  = n;
	_unlock_emul_atomic();
	return ret;
}

int32_t _phongo_emul_atomic_int32_compare_exchange_strong(
	volatile int32_t*        p,
	int32_t                  expect_value,
	int32_t                  new_value,
	enum phongo_memory_order _unused)
{
	int32_t ret;

	BSON_UNUSED(_unused);

	_lock_emul_atomic();
	ret = *p;
	if (ret == expect_value) {
		*p = new_value;
	}
	_unlock_emul_atomic();
	return ret;
}

int32_t _phongo_emul_atomic_int32_compare_exchange_weak(
	volatile int32_t*        p,
	int32_t                  expect_value,
	int32_t                  new_value,
	enum phongo_memory_order order)
{
	/* We're emulating. We can't do a weak version. */
	return _phongo_emul_atomic_int32_compare_exchange_strong(p, expect_value, new_value, order);
}

/*
 * Copyright 2015-present MongoDB, Inc.
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

#include <php.h>
#include <ext/spl/spl_exceptions.h>

#include "php_phongo.h"
#include "InvalidArgumentException_arginfo.h"

zend_class_entry* php_phongo_invalidargumentexception_ce;

void php_phongo_invalidargumentexception_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_invalidargumentexception_ce = register_class_MongoDB_Driver_Exception_InvalidArgumentException(spl_ce_InvalidArgumentException, php_phongo_exception_ce);
}

/*
 * Copyright 2013 MongoDB Inc.
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


#ifndef MONGOC_CONFIG_H
#define MONGOC_CONFIG_H


/*
 * MONGOC_ENABLE_SSL_SECURE_CHANNEL is set from configure to determine if we are
 * compiled with Native SSL support on Windows
 */
#define MONGOC_ENABLE_SSL_SECURE_CHANNEL 1

#if MONGOC_ENABLE_SSL_SECURE_CHANNEL != 1
#  undef MONGOC_ENABLE_SSL_SECURE_CHANNEL
#endif


/*
 * MONGOC_ENABLE_CRYPTO_CNG is set from configure to determine if we are
 * compiled with Native Crypto support on Windows
 */
#define MONGOC_ENABLE_CRYPTO_CNG 1

#if MONGOC_ENABLE_CRYPTO_CNG != 1
#  undef MONGOC_ENABLE_CRYPTO_CNG
#endif


/*
 * MONGOC_ENABLE_SSL_SECURE_TRANSPORT is set from configure to determine if we are
 * compiled with Native SSL support on Darwin
 */
#define MONGOC_ENABLE_SSL_SECURE_TRANSPORT 0

#if MONGOC_ENABLE_SSL_SECURE_TRANSPORT != 1
#  undef MONGOC_ENABLE_SSL_SECURE_TRANSPORT
#endif


/*
 * MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO is set from configure to determine if we are
 * compiled with Native Crypto support on Darwin
 */
#define MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO 0

#if MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO != 1
#  undef MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO
#endif


/*
 * MONGOC_ENABLE_SSL_OPENSSL is set from configure to determine if we are
 * compiled with OpenSSL support.
 */
#define MONGOC_ENABLE_SSL_OPENSSL 0

#if MONGOC_ENABLE_SSL_OPENSSL != 1
#  undef MONGOC_ENABLE_SSL_OPENSSL
#endif


/*
 * MONGOC_ENABLE_CRYPTO_LIBCRYPTO is set from configure to determine if we are
 * compiled with OpenSSL support.
 */
#define MONGOC_ENABLE_CRYPTO_LIBCRYPTO 0

#if MONGOC_ENABLE_CRYPTO_LIBCRYPTO != 1
#  undef MONGOC_ENABLE_CRYPTO_LIBCRYPTO
#endif


/*
 * MONGOC_ENABLE_SSL is set from configure to determine if we are
 * compiled with any SSL support.
 */
#define MONGOC_ENABLE_SSL 1

#if MONGOC_ENABLE_SSL != 1
#  undef MONGOC_ENABLE_SSL
#endif


/*
 * MONGOC_ENABLE_CRYPTO is set from configure to determine if we are
 * compiled with any crypto support.
 */
#define MONGOC_ENABLE_CRYPTO 1

#if MONGOC_ENABLE_CRYPTO != 1
#  undef MONGOC_ENABLE_CRYPTO
#endif


/*
 * Use system crypto profile
 */
#define MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE 0

#if MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE != 1
#  undef MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE
#endif


/*
 * MONGOC_ENABLE_SASL is set from configure to determine if we are
 * compiled with SASL support.
 */
#define MONGOC_ENABLE_SASL 1

#if MONGOC_ENABLE_SASL != 1
#  undef MONGOC_ENABLE_SASL
#endif


/*
 * MONGOC_HAVE_SASL_CLIENT_DONE is set from configure to determine if we
 * have SASL and its version is new enough to use sasl_client_done (),
 * which supersedes sasl_done ().
 */
#define MONGOC_HAVE_SASL_CLIENT_DONE 1

#if MONGOC_HAVE_SASL_CLIENT_DONE != 1
#  undef MONGOC_HAVE_SASL_CLIENT_DONE
#endif


/*
 * MONGOC_HAVE_WEAK_SYMBOLS is set from configure to determine if the
 * compiler supports the (weak) annotation. We use it to prevent
 * Link-Time-Optimization (LTO) in our constant-time mongoc_memcmp()
 * This is known to work with GNU GCC and Solaris Studio
 */
#define MONGOC_HAVE_WEAK_SYMBOLS 1

#if MONGOC_HAVE_WEAK_SYMBOLS != 1
#  undef MONGOC_HAVE_WEAK_SYMBOLS
#endif


/*
 * Disable automatic calls to mongoc_init() and mongoc_cleanup()
 * before main() is called, and after exit() (respectively).
 */
#define MONGOC_NO_AUTOMATIC_GLOBALS 1

#if MONGOC_NO_AUTOMATIC_GLOBALS != 1
#  undef MONGOC_NO_AUTOMATIC_GLOBALS
#endif

#endif /* MONGOC_CONFIG_H */

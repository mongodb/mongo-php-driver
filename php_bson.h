/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2014 Hannes Magnusson                                  |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Hannes Magnusson <bjori@php.net>                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHONGO_BSON_H
#define PHONGO_BSON_H

#include <bson.h>

/* PHP Core stuff */
#include <php.h>

/* Our stuff */
#include "php_phongo.h"

PHONGO_API int php_phongo_is_array_or_document(zval **val TSRMLS_DC);
PHONGO_API void php_phongo_bson_encode_array(bson_t *bson, zval *data TSRMLS_DC);
PHONGO_API void php_phongo_bson_encode(bson_t *bson, const char *key, int key_len, zval *val TSRMLS_DC);
PHONGO_API int bson_to_zval(const unsigned char *data, int data_len, zval *retval);

PHP_FUNCTION(bson_encode);
PHP_FUNCTION(bson_decode);
PHP_FUNCTION(bson_to_json);

#endif	/* PHONGO_BSON_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

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

typedef enum {
	PHONGO_BSON_NONE      = 0x00,
	PHONGO_BSON_ADD_ID    = 0x01,
	PHONGO_BSON_RETURN_ID = 0x02,
	PHONGO_BSON_ADD_ODS   = 0x04,
	PHONGO_BSON_ADD_CHILD_ODS = 0x08
} phongo_bson_flags_t;

PHONGO_API void zval_to_bson(zval *data, phongo_bson_flags_t flags, bson_t *bson, bson_t **bson_out TSRMLS_DC);
PHONGO_API int bson_to_zval(const unsigned char *data, int data_len, php_phongo_bson_state *state);
PHONGO_API void php_phongo_bson_typemap_to_state(zval *typemap, php_phongo_bson_typemap *map TSRMLS_DC);

PHP_FUNCTION(toArray);
PHP_FUNCTION(fromArray);
PHP_FUNCTION(toJSON);
PHP_FUNCTION(fromJSON);

#endif	/* PHONGO_BSON_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

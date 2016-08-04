/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 20142015 Hannes Magnusson                              |
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

#define BSON_UNSERIALIZE_FUNC_NAME    "bsonUnserialize"
#define BSON_SERIALIZE_FUNC_NAME      "bsonSerialize"

typedef enum {
	PHONGO_BSON_NONE      = 0x00,
	PHONGO_BSON_ADD_ID    = 0x01,
	PHONGO_BSON_RETURN_ID = 0x02
} php_phongo_bson_flags_t;

typedef enum {
	PHONGO_TYPEMAP_NONE,
	PHONGO_TYPEMAP_NATIVE_ARRAY,
	PHONGO_TYPEMAP_NATIVE_OBJECT,
	PHONGO_TYPEMAP_CLASS
} php_phongo_bson_typemap_types;

typedef struct {
	php_phongo_bson_typemap_types  document_type;
	zend_class_entry              *document;
	php_phongo_bson_typemap_types  array_type;
	zend_class_entry              *array;
	php_phongo_bson_typemap_types  root_type;
	zend_class_entry              *root;
} php_phongo_bson_typemap;

typedef struct {
#if PHP_VERSION_ID >= 70000
	zval                     zchild;
#else
	zval                    *zchild;
#endif
	php_phongo_bson_typemap  map;
	zend_class_entry        *odm;
} php_phongo_bson_state;

#if PHP_VERSION_ID >= 70000
#define PHONGO_BSON_STATE_INITIALIZER  { {{ 0 }}, { PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL}, NULL}
#else
#define PHONGO_BSON_STATE_INITIALIZER  { NULL, { PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL}, NULL}
#endif

PHONGO_API void phongo_zval_to_bson(zval *data, php_phongo_bson_flags_t flags, bson_t *bson, bson_t **bson_out TSRMLS_DC);
PHONGO_API int phongo_bson_to_zval_ex(const unsigned char *data, int data_len, php_phongo_bson_state *state);
#if PHP_VERSION_ID >= 70000
PHONGO_API int phongo_bson_to_zval(const unsigned char *data, int data_len, zval *out);
#else
PHONGO_API int phongo_bson_to_zval(const unsigned char *data, int data_len, zval **out);
#endif
PHONGO_API void phongo_bson_typemap_to_state(zval *typemap, php_phongo_bson_typemap *map TSRMLS_DC);

PHP_FUNCTION(toPHP);
PHP_FUNCTION(fromPHP);
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

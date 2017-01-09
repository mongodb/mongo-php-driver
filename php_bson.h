/*
 * Copyright 2014-2017 MongoDB, Inc.
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
	bool                     is_visiting_array;
} php_phongo_bson_state;

#if PHP_VERSION_ID >= 70000
#define PHONGO_BSON_STATE_INITIALIZER  { {{ 0 }}, { PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL}, NULL, 0 }
#else
#define PHONGO_BSON_STATE_INITIALIZER  { NULL, { PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL, PHONGO_TYPEMAP_NONE, NULL}, NULL, 0 }
#endif

void phongo_zval_to_bson(zval *data, php_phongo_bson_flags_t flags, bson_t *bson, bson_t **bson_out TSRMLS_DC);
int phongo_bson_to_zval_ex(const unsigned char *data, int data_len, php_phongo_bson_state *state);
#if PHP_VERSION_ID >= 70000
int phongo_bson_to_zval(const unsigned char *data, int data_len, zval *out);
#else
int phongo_bson_to_zval(const unsigned char *data, int data_len, zval **out);
#endif
void phongo_bson_typemap_to_state(zval *typemap, php_phongo_bson_typemap *map TSRMLS_DC);

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

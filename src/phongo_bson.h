/*
 * Copyright 2022-present MongoDB, Inc.
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

#include "bson/bson.h"

#include <php.h>

#define BSON_UNSERIALIZE_FUNC_NAME "bsonUnserialize"
#define BSON_SERIALIZE_FUNC_NAME "bsonSerialize"
#define PHONGO_ODM_FIELD_NAME "__pclass"

typedef enum {
	PHONGO_FIELD_PATH_ITEM_NONE,
	PHONGO_FIELD_PATH_ITEM_ARRAY,
	PHONGO_FIELD_PATH_ITEM_DOCUMENT
} php_phongo_bson_field_path_item_types;

typedef struct {
	char**                                 elements;
	php_phongo_bson_field_path_item_types* element_types;
	size_t                                 allocated_size;
	size_t                                 size;
	size_t                                 ref_count;
	bool                                   owns_elements;
} php_phongo_field_path;

typedef enum {
	PHONGO_TYPEMAP_NONE,
	PHONGO_TYPEMAP_NATIVE_ARRAY,
	PHONGO_TYPEMAP_NATIVE_OBJECT,
	PHONGO_TYPEMAP_CLASS,
	PHONGO_TYPEMAP_BSON
} php_phongo_bson_typemap_types;

typedef struct {
	php_phongo_bson_typemap_types type;
	zend_class_entry*             ce;
} php_phongo_bson_typemap_element;

typedef struct {
	php_phongo_field_path*          entry;
	php_phongo_bson_typemap_element node;
} php_phongo_field_path_map_element;

typedef struct {
	php_phongo_bson_typemap_element document;
	php_phongo_bson_typemap_element array;
	php_phongo_bson_typemap_element root;
	bool                            int64_as_object;
	struct {
		php_phongo_field_path_map_element** map;
		size_t                              allocated_size;
		size_t                              size;
	} field_paths;
} php_phongo_bson_typemap;

typedef struct {
	zval                            zchild;
	php_phongo_bson_typemap         map;
	zend_class_entry*               odm_ce;
	bool                            is_visiting_array;
	php_phongo_field_path*          field_path;
	php_phongo_bson_typemap_element field_type;
} php_phongo_bson_state;

typedef enum {
	PHONGO_JSON_MODE_LEGACY,
	PHONGO_JSON_MODE_CANONICAL,
	PHONGO_JSON_MODE_RELAXED,
} php_phongo_json_mode_t;

#define PHONGO_BSON_INIT_STATE(s)                       \
	do {                                                \
		memset(&(s), 0, sizeof(php_phongo_bson_state)); \
	} while (0)

#define PHONGO_BSON_INIT_DEBUG_STATE(s)                    \
	do {                                                   \
		memset(&(s), 0, sizeof(php_phongo_bson_state));    \
		s.map.root.type     = PHONGO_TYPEMAP_NATIVE_ARRAY; \
		s.map.document.type = PHONGO_TYPEMAP_NATIVE_ARRAY; \
	} while (0)

char*                  php_phongo_field_path_as_string(php_phongo_field_path* field_path);
php_phongo_field_path* php_phongo_field_path_alloc(bool owns_elements);
void                   php_phongo_field_path_free(php_phongo_field_path* field_path);
void                   php_phongo_field_path_write_item_at_current_level(php_phongo_field_path* field_path, const char* element);
void                   php_phongo_field_path_write_type_at_current_level(php_phongo_field_path* field_path, php_phongo_bson_field_path_item_types element_type);
bool                   php_phongo_field_path_push(php_phongo_field_path* field_path, const char* element, php_phongo_bson_field_path_item_types element_type);
bool                   php_phongo_field_path_pop(php_phongo_field_path* field_path);

bool php_phongo_bson_to_json(zval* return_value, const bson_t* bson, php_phongo_json_mode_t mode);
bool php_phongo_bson_to_zval(const bson_t* b, zval* zv);
bool php_phongo_bson_to_zval_ex(const bson_t* b, php_phongo_bson_state* state);
bool php_phongo_bson_data_to_zval(const unsigned char* data, int data_len, zval* zv);
bool php_phongo_bson_data_to_zval_ex(const unsigned char* data, int data_len, php_phongo_bson_state* state);

bool phongo_bson_value_to_zval(const bson_value_t* value, zval* zv);
bool phongo_bson_value_to_zval_legacy(const bson_value_t* value, zval* zv);

bool php_phongo_bson_typemap_to_state(zval* typemap, php_phongo_bson_typemap* map);
void php_phongo_bson_typemap_dtor(php_phongo_bson_typemap* map);

#endif /* PHONGO_BSON_H */

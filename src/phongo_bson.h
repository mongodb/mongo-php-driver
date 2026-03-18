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
#define BSON_MAX_NESTING_LEVEL 100

typedef enum {
	PHONGO_FIELD_PATH_ITEM_NONE,
	PHONGO_FIELD_PATH_ITEM_ARRAY,
	PHONGO_FIELD_PATH_ITEM_DOCUMENT
} phongo_bson_field_path_item_types;

typedef struct {
	char**                             elements;
	phongo_bson_field_path_item_types* element_types;
	size_t                             allocated_size;
	size_t                             size;
	size_t                             ref_count;
	bool                               owns_elements;
} phongo_field_path;

typedef enum {
	PHONGO_TYPEMAP_NONE,
	PHONGO_TYPEMAP_NATIVE_ARRAY,
	PHONGO_TYPEMAP_NATIVE_OBJECT,
	PHONGO_TYPEMAP_CLASS,
	PHONGO_TYPEMAP_BSON
} phongo_bson_typemap_types;

typedef struct {
	phongo_bson_typemap_types type;
	zend_class_entry*         ce;
} phongo_bson_typemap_element;

typedef struct {
	phongo_field_path*          entry;
	phongo_bson_typemap_element node;
} phongo_field_path_map_element;

typedef struct {
	phongo_bson_typemap_element document;
	phongo_bson_typemap_element array;
	phongo_bson_typemap_element root;
	bool                        int64_as_object;
	struct {
		phongo_field_path_map_element** map;
		size_t                          allocated_size;
		size_t                          size;
	} field_paths;
} phongo_bson_typemap;

typedef struct {
	zval                        zchild;
	phongo_bson_typemap         map;
	zend_class_entry*           odm_ce;
	bool                        is_visiting_array;
	phongo_field_path*          field_path;
	phongo_bson_typemap_element field_type;
} phongo_bson_state;

typedef enum {
	PHONGO_JSON_MODE_LEGACY,
	PHONGO_JSON_MODE_CANONICAL,
	PHONGO_JSON_MODE_RELAXED,
} phongo_json_mode_t;

#define PHONGO_BSON_INIT_STATE(s)                   \
	do {                                            \
		memset(&(s), 0, sizeof(phongo_bson_state)); \
	} while (0)

#define PHONGO_BSON_INIT_DEBUG_STATE(s)                    \
	do {                                                   \
		memset(&(s), 0, sizeof(phongo_bson_state));        \
		s.map.root.type     = PHONGO_TYPEMAP_NATIVE_ARRAY; \
		s.map.document.type = PHONGO_TYPEMAP_NATIVE_ARRAY; \
	} while (0)

char*              phongo_field_path_as_string(phongo_field_path* field_path);
phongo_field_path* phongo_field_path_alloc(bool owns_elements);
void               phongo_field_path_free(phongo_field_path* field_path);
void               phongo_field_path_write_item_at_current_level(phongo_field_path* field_path, const char* element);
void               phongo_field_path_write_type_at_current_level(phongo_field_path* field_path, phongo_bson_field_path_item_types element_type);
bool               phongo_field_path_push(phongo_field_path* field_path, const char* element, phongo_bson_field_path_item_types element_type);
bool               phongo_field_path_pop(phongo_field_path* field_path);

bool phongo_bson_to_json(zval* return_value, const bson_t* bson, phongo_json_mode_t mode);
bool phongo_bson_to_zval(const bson_t* b, zval* zv);
bool phongo_bson_to_zval_ex(const bson_t* b, phongo_bson_state* state);
bool phongo_bson_data_to_zval(const unsigned char* data, int data_len, zval* zv);
bool phongo_bson_data_to_zval_ex(const unsigned char* data, int data_len, phongo_bson_state* state);

bool phongo_bson_value_to_zval(const bson_value_t* value, zval* zv);
bool phongo_bson_value_to_zval_legacy(const bson_value_t* value, zval* zv);

bool phongo_bson_typemap_to_state(zval* typemap, phongo_bson_typemap* map);
void phongo_bson_typemap_dtor(phongo_bson_typemap* map);

#endif /* PHONGO_BSON_H */

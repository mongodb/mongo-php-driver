/*
 * Copyright 2025-present MongoDB, Inc.
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
#include <Zend/zend_enum.h>

#include "phongo.h"
#include "VectorType.h"
#include "VectorType_arginfo.h"

zend_class_entry* phongo_vectortype_ce;

phongo_bson_vector_type_t phongo_bson_vector_type_from_name(const char* name)
{
	if (!strcmp(name, PHONGO_BSON_VECTOR_TYPE_FLOAT32_NAME)) {
		return PHONGO_BSON_VECTOR_TYPE_FLOAT32;
	}

	if (!strcmp(name, PHONGO_BSON_VECTOR_TYPE_INT8_NAME)) {
		return PHONGO_BSON_VECTOR_TYPE_INT8;
	}

	if (!strcmp(name, PHONGO_BSON_VECTOR_TYPE_PACKED_BIT_NAME)) {
		return PHONGO_BSON_VECTOR_TYPE_PACKED_BIT;
	}

	return PHONGO_BSON_VECTOR_TYPE_UNKNOWN;
}

const char* phongo_bson_vector_type_to_name(phongo_bson_vector_type_t type)
{
	switch (type) {
		case PHONGO_BSON_VECTOR_TYPE_FLOAT32:
			return PHONGO_BSON_VECTOR_TYPE_FLOAT32_NAME;
		case PHONGO_BSON_VECTOR_TYPE_INT8:
			return PHONGO_BSON_VECTOR_TYPE_INT8_NAME;
		case PHONGO_BSON_VECTOR_TYPE_PACKED_BIT:
			return PHONGO_BSON_VECTOR_TYPE_PACKED_BIT_NAME;
		default:
			return NULL;
	}
}

phongo_bson_vector_type_t phongo_bson_vector_type_from_case(zend_object* case_obj)
{
	return phongo_bson_vector_type_from_name(Z_STRVAL_P(zend_enum_fetch_case_name(case_obj)));
}

zend_object* phongo_bson_vector_type_to_case(phongo_bson_vector_type_t type)
{
	const char* name = phongo_bson_vector_type_to_name(type);

	if (!name) {
		return NULL;
	}

	return zend_enum_get_case_cstr(phongo_vectortype_ce, name);
}

void phongo_vectortype_init_ce(INIT_FUNC_ARGS)
{
	phongo_vectortype_ce = register_class_MongoDB_BSON_VectorType();
}

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

#ifndef PHONGO_BSON_VECTORTYPE_H
#define PHONGO_BSON_VECTORTYPE_H

#define PHONGO_BSON_VECTOR_TYPE_FLOAT32_NAME "Float32"
#define PHONGO_BSON_VECTOR_TYPE_INT8_NAME "Int8"
#define PHONGO_BSON_VECTOR_TYPE_PACKED_BIT_NAME "PackedBit"

typedef enum {
	PHONGO_BSON_VECTOR_TYPE_FLOAT32    = 0x27,
	PHONGO_BSON_VECTOR_TYPE_INT8       = 0x03,
	PHONGO_BSON_VECTOR_TYPE_PACKED_BIT = 0x10,
	PHONGO_BSON_VECTOR_TYPE_UNKNOWN    = 0,
} phongo_bson_vector_type_t;

phongo_bson_vector_type_t phongo_bson_vector_type_from_name(const char* name);
const char*               phongo_bson_vector_type_to_name(phongo_bson_vector_type_t type);
phongo_bson_vector_type_t phongo_bson_vector_type_from_case(zend_object* case_obj);
zend_object*              phongo_bson_vector_type_to_case(phongo_bson_vector_type_t type);

#endif /* PHONGO_BSON_VECTORTYPE_H */

/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014, MongoDB, Inc.                                         |
  +---------------------------------------------------------------------------+
*/

/* External libs */
#include <bson.h>
#include <mongoc.h>

#include "pongoc.h"

#define MONGOC_INSIDE
#include "src/libmongoc/src/mongoc/mongoc-stream-private.h"
#include "src/libmongoc/src/mongoc/mongoc-write-command-private.h"
#include "src/libmongoc/src/mongoc/mongoc-write-command-private.h"
#include "src/libmongoc/src/mongoc/mongoc-array-private.h"
#include "src/libmongoc/src/mongoc/mongoc-bulk-operation-private.h"
#undef MONGOC_INSIDE


void
_mongoc_write_result_init (mongoc_write_result_t *result) /* IN */
{
   BSON_ASSERT (result);

   memset (result, 0, sizeof *result);

   bson_init (&result->upserted);
   bson_init (&result->writeConcernErrors);
   bson_init (&result->writeErrors);
}
void
_mongoc_array_init (mongoc_array_t *array,
                    size_t          element_size)
{
   bson_return_if_fail(array);
   bson_return_if_fail(element_size);

   array->len = 0;
   array->element_size = element_size;
   array->allocated = 128;
   array->data = bson_malloc0(array->allocated);
}

mongoc_bulk_operation_t *
_mongoc_bulk_operation_allocate ()
{
	mongoc_bulk_operation_t *batch;

	batch = bson_malloc0 (sizeof *batch);

	batch->executed = false;
	batch->hint = 0;

	_mongoc_write_result_init (&batch->result);
	_mongoc_array_init (&batch->commands, sizeof (mongoc_write_command_t));

	return batch;
}


void
_mongoc_bulk_operation_set (mongoc_bulk_operation_t *bulk,                /* IN */
		mongoc_client_t              *client,        /* IN */
		const char                   *database,      /* IN */
		const char                   *collection,    /* IN */
		bool                          ordered,       /* IN */
		const mongoc_write_concern_t *write_concern) /* IN */
{
	bulk->client = client;
	bulk->database = bson_strdup(database);
	bulk->collection = bson_strdup(collection);
	bulk->ordered = ordered;

	bulk->write_concern = mongoc_write_concern_copy (write_concern);
	if (!bulk->write_concern) {
		bulk->write_concern = mongoc_write_concern_new ();
	}
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */


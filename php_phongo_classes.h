/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2014-2015 Hannes Magnusson                             |
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

#ifndef PHONGO_CLASSES_H
#define PHONGO_CLASSES_H

/* PHP Core stuff */
#include <php.h>
#include <mongoc-bulk-operation-private.h>

/* Our stuffz */
#include "php_bson.h"


typedef struct {
	zend_object              std;
	bson_t                  *bson;
} php_phongo_command_t;

typedef struct {
	zend_object              std;
	mongoc_cursor_t         *cursor;
	mongoc_client_t         *client;
	int                      server_id;
	php_phongo_bson_state    visitor_data;
	int                      got_iterator;
} php_phongo_cursor_t;

typedef struct {
	zend_object_iterator   intern;
	php_phongo_cursor_t   *cursor;
	long                   current;
} php_phongo_cursor_iterator;

typedef struct {
	zend_object              std;
	uint64_t                 id;
} php_phongo_cursorid_t;

typedef struct {
	zend_object              std;
	mongoc_client_t         *client;
} php_phongo_manager_t;

typedef struct {
	zend_object              std;
	bson_t                  *query;
	bson_t                  *selector;
	mongoc_query_flags_t     flags;
	uint32_t                 skip;
	uint32_t                 limit;
	uint32_t                 batch_size;
} php_phongo_query_t;

typedef struct {
	zend_object              std;
	mongoc_read_prefs_t     *read_preference;
} php_phongo_readpreference_t;

typedef struct {
	zend_object              std;
	mongoc_client_t         *client;
	int                      server_id;
} php_phongo_server_t;

typedef struct {
	zend_object              std;
	mongoc_bulk_operation_t *bulk;
} php_phongo_bulkwrite_t;

typedef struct {
	zend_object              std;
	mongoc_write_concern_t  *write_concern;
} php_phongo_writeconcern_t;

typedef struct {
	zend_object              std;
	int                      code;
	char                    *message;
	zval                    *info;
} php_phongo_writeconcernerror_t;

typedef struct {
	zend_object              std;
	int                      code;
	char                    *message;
	zval                    *info;
	uint32_t                 index;
} php_phongo_writeerror_t;

typedef struct {
	zend_object              std;
	mongoc_write_concern_t  *write_concern;
	mongoc_write_result_t    write_result;
	mongoc_client_t         *client;
	int                      server_id;
} php_phongo_writeresult_t;

typedef struct {
	zend_object              std;
	char                    *data;
	int                      data_len;
	int                      type;
} php_phongo_binary_t;
typedef struct {
	zend_object              std;
} php_phongo_int32_t;
typedef struct {
	zend_object              std;
} php_phongo_int64_t;
typedef struct {
	zend_object              std;
	char                    *javascript;
	size_t                   javascript_len;
	bson_t                  *document;
} php_phongo_javascript_t;
typedef struct {
	zend_object              std;
} php_phongo_log_t;
typedef struct {
	zend_object              std;
} php_phongo_maxkey_t;
typedef struct {
	zend_object              std;
} php_phongo_minkey_t;
typedef struct {
	zend_object              std;
	char                     oid[25];
} php_phongo_objectid_t;
typedef struct {
	zend_object              std;
	char                    *pattern;
	int                      pattern_len;
	char                    *flags;
	int                      flags_len;
} php_phongo_regex_t;
typedef struct {
	zend_object              std;
	int32_t                  increment;
	int32_t                  timestamp;
} php_phongo_timestamp_t;
typedef struct {
	zend_object              std;
	int64_t                  milliseconds;
} php_phongo_utcdatetime_t;

extern PHONGO_API zend_class_entry *php_phongo_command_ce;
extern PHONGO_API zend_class_entry *php_phongo_cursor_ce;
extern PHONGO_API zend_class_entry *php_phongo_cursorid_ce;
extern PHONGO_API zend_class_entry *php_phongo_manager_ce;
extern PHONGO_API zend_class_entry *php_phongo_query_ce;
extern PHONGO_API zend_class_entry *php_phongo_readpreference_ce;
extern PHONGO_API zend_class_entry *php_phongo_result_ce;
extern PHONGO_API zend_class_entry *php_phongo_server_ce;
extern PHONGO_API zend_class_entry *php_phongo_bulkwrite_ce;
extern PHONGO_API zend_class_entry *php_phongo_writeconcern_ce;
extern PHONGO_API zend_class_entry *php_phongo_writeconcernerror_ce;
extern PHONGO_API zend_class_entry *php_phongo_writeerror_ce;
extern PHONGO_API zend_class_entry *php_phongo_writeresult_ce;

extern PHONGO_API zend_class_entry *php_phongo_exception_ce;
extern PHONGO_API zend_class_entry *php_phongo_logicexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_runtimeexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_unexpectedvalueexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_invalidargumentexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_connectionexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_authenticationexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_sslconnectionexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_executiontimeoutexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_connectiontimeoutexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_writeexception_ce;
extern PHONGO_API zend_class_entry *php_phongo_bulkwriteexception_ce;

extern PHONGO_API zend_class_entry *php_phongo_type_ce;
extern PHONGO_API zend_class_entry *php_phongo_persistable_ce;
extern PHONGO_API zend_class_entry *php_phongo_unserializable_ce;
extern PHONGO_API zend_class_entry *php_phongo_serializable_ce;
extern PHONGO_API zend_class_entry *php_phongo_binary_ce;
extern PHONGO_API zend_class_entry *php_phongo_int32_ce;
extern PHONGO_API zend_class_entry *php_phongo_int64_ce;
extern PHONGO_API zend_class_entry *php_phongo_javascript_ce;
extern PHONGO_API zend_class_entry *php_phongo_log_ce;
extern PHONGO_API zend_class_entry *php_phongo_maxkey_ce;
extern PHONGO_API zend_class_entry *php_phongo_minkey_ce;
extern PHONGO_API zend_class_entry *php_phongo_objectid_ce;
extern PHONGO_API zend_class_entry *php_phongo_regex_ce;
extern PHONGO_API zend_class_entry *php_phongo_timestamp_ce;
extern PHONGO_API zend_class_entry *php_phongo_utcdatetime_ce;

/* Shared across all MongoDB\Driver objects to disable unserialize() */
PHP_METHOD(Manager, __wakeUp);
/* Shared across all final MongoDB\Driver value objects, only possible to construct them internally */
PHP_METHOD(Server, __construct);

PHP_MINIT_FUNCTION(Command);
PHP_MINIT_FUNCTION(Cursor);
PHP_MINIT_FUNCTION(CursorId);
PHP_MINIT_FUNCTION(Manager);
PHP_MINIT_FUNCTION(Query);
PHP_MINIT_FUNCTION(ReadPreference);
PHP_MINIT_FUNCTION(Result);
PHP_MINIT_FUNCTION(Server);
PHP_MINIT_FUNCTION(BulkWrite);
PHP_MINIT_FUNCTION(WriteConcern);
PHP_MINIT_FUNCTION(WriteConcernError);
PHP_MINIT_FUNCTION(WriteError);
PHP_MINIT_FUNCTION(WriteResult);

PHP_MINIT_FUNCTION(Exception);
PHP_MINIT_FUNCTION(LogicException);
PHP_MINIT_FUNCTION(RuntimeException);
PHP_MINIT_FUNCTION(UnexpectedValueException);
PHP_MINIT_FUNCTION(InvalidArgumentException);
PHP_MINIT_FUNCTION(ConnectionException);
PHP_MINIT_FUNCTION(AuthenticationException);
PHP_MINIT_FUNCTION(SSLConnectionException);
PHP_MINIT_FUNCTION(ExecutionTimeoutException);
PHP_MINIT_FUNCTION(ConnectionTimeoutException);
PHP_MINIT_FUNCTION(WriteException);
PHP_MINIT_FUNCTION(BulkWriteException);

PHP_MINIT_FUNCTION(Type);
PHP_MINIT_FUNCTION(Unserializable);
PHP_MINIT_FUNCTION(Serializable);
PHP_MINIT_FUNCTION(Persistable);
PHP_MINIT_FUNCTION(Binary);
PHP_MINIT_FUNCTION(Javascript);
PHP_MINIT_FUNCTION(MaxKey);
PHP_MINIT_FUNCTION(MinKey);
PHP_MINIT_FUNCTION(ObjectID);
PHP_MINIT_FUNCTION(Regex);
PHP_MINIT_FUNCTION(Timestamp);
PHP_MINIT_FUNCTION(UTCDateTime);

#endif /* PHONGO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

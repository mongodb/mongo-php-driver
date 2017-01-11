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

#ifndef PHONGO_CLASSES_H
#define PHONGO_CLASSES_H

#include "php_phongo_structs.h"

/* Export zend_class_entry dependencies, which are initialized in MINIT */
extern zend_class_entry *php_phongo_date_immutable_ce;
extern zend_class_entry *php_phongo_json_serializable_ce;

#if PHP_VERSION_ID >= 70000

static inline php_phongo_bulkwrite_t* php_bulkwrite_fetch_object(zend_object *obj) {
    return (php_phongo_bulkwrite_t *)((char *)obj - XtOffsetOf(php_phongo_bulkwrite_t, std));
}
static inline php_phongo_command_t* php_command_fetch_object(zend_object *obj) {
    return (php_phongo_command_t *)((char *)obj - XtOffsetOf(php_phongo_command_t, std));
}
static inline php_phongo_cursor_t* php_cursor_fetch_object(zend_object *obj) {
    return (php_phongo_cursor_t *)((char *)obj - XtOffsetOf(php_phongo_cursor_t, std));
}
static inline php_phongo_cursorid_t* php_cursorid_fetch_object(zend_object *obj) {
    return (php_phongo_cursorid_t *)((char *)obj - XtOffsetOf(php_phongo_cursorid_t, std));
}
static inline php_phongo_manager_t* php_manager_fetch_object(zend_object *obj) {
    return (php_phongo_manager_t *)((char *)obj - XtOffsetOf(php_phongo_manager_t, std));
}
static inline php_phongo_query_t* php_query_fetch_object(zend_object *obj) {
    return (php_phongo_query_t *)((char *)obj - XtOffsetOf(php_phongo_query_t, std));
}
static inline php_phongo_readconcern_t* php_readconcern_fetch_object(zend_object *obj) {
    return (php_phongo_readconcern_t *)((char *)obj - XtOffsetOf(php_phongo_readconcern_t, std));
}
static inline php_phongo_readpreference_t* php_readpreference_fetch_object(zend_object *obj) {
    return (php_phongo_readpreference_t *)((char *)obj - XtOffsetOf(php_phongo_readpreference_t, std));
}
static inline php_phongo_server_t* php_server_fetch_object(zend_object *obj) {
    return (php_phongo_server_t *)((char *)obj - XtOffsetOf(php_phongo_server_t, std));
}
static inline php_phongo_writeconcern_t* php_writeconcern_fetch_object(zend_object *obj) {
    return (php_phongo_writeconcern_t *)((char *)obj - XtOffsetOf(php_phongo_writeconcern_t, std));
}
static inline php_phongo_writeconcernerror_t* php_writeconcernerror_fetch_object(zend_object *obj) {
    return (php_phongo_writeconcernerror_t *)((char *)obj - XtOffsetOf(php_phongo_writeconcernerror_t, std));
}
static inline php_phongo_writeerror_t* php_writeerror_fetch_object(zend_object *obj) {
    return (php_phongo_writeerror_t *)((char *)obj - XtOffsetOf(php_phongo_writeerror_t, std));
}
static inline php_phongo_writeresult_t* php_writeresult_fetch_object(zend_object *obj) {
    return (php_phongo_writeresult_t *)((char *)obj - XtOffsetOf(php_phongo_writeresult_t, std));
}
static inline php_phongo_binary_t* php_binary_fetch_object(zend_object *obj) {
    return (php_phongo_binary_t *)((char *)obj - XtOffsetOf(php_phongo_binary_t, std));
}
static inline php_phongo_decimal128_t* php_decimal128_fetch_object(zend_object *obj) {
    return (php_phongo_decimal128_t *)((char *)obj - XtOffsetOf(php_phongo_decimal128_t, std));
}
static inline php_phongo_javascript_t* php_javascript_fetch_object(zend_object *obj) {
    return (php_phongo_javascript_t *)((char *)obj - XtOffsetOf(php_phongo_javascript_t, std));
}
static inline php_phongo_maxkey_t* php_maxkey_fetch_object(zend_object *obj) {
    return (php_phongo_maxkey_t *)((char *)obj - XtOffsetOf(php_phongo_maxkey_t, std));
}
static inline php_phongo_minkey_t* php_minkey_fetch_object(zend_object *obj) {
    return (php_phongo_minkey_t *)((char *)obj - XtOffsetOf(php_phongo_minkey_t, std));
}
static inline php_phongo_objectid_t* php_objectid_fetch_object(zend_object *obj) {
    return (php_phongo_objectid_t *)((char *)obj - XtOffsetOf(php_phongo_objectid_t, std));
}
static inline php_phongo_regex_t* php_regex_fetch_object(zend_object *obj) {
    return (php_phongo_regex_t *)((char *)obj - XtOffsetOf(php_phongo_regex_t, std));
}
static inline php_phongo_timestamp_t* php_timestamp_fetch_object(zend_object *obj) {
    return (php_phongo_timestamp_t *)((char *)obj - XtOffsetOf(php_phongo_timestamp_t, std));
}
static inline php_phongo_utcdatetime_t* php_utcdatetime_fetch_object(zend_object *obj) {
    return (php_phongo_utcdatetime_t *)((char *)obj - XtOffsetOf(php_phongo_utcdatetime_t, std));
}

# define Z_COMMAND_OBJ_P(zv)           (php_command_fetch_object(Z_OBJ_P(zv)))
# define Z_CURSOR_OBJ_P(zv)            (php_cursor_fetch_object(Z_OBJ_P(zv)))
# define Z_CURSORID_OBJ_P(zv)          (php_cursorid_fetch_object(Z_OBJ_P(zv)))
# define Z_MANAGER_OBJ_P(zv)           (php_manager_fetch_object(Z_OBJ_P(zv)))
# define Z_QUERY_OBJ_P(zv)             (php_query_fetch_object(Z_OBJ_P(zv)))
# define Z_READCONCERN_OBJ_P(zv)       (php_readconcern_fetch_object(Z_OBJ_P(zv)))
# define Z_READPREFERENCE_OBJ_P(zv)    (php_readpreference_fetch_object(Z_OBJ_P(zv)))
# define Z_SERVER_OBJ_P(zv)            (php_server_fetch_object(Z_OBJ_P(zv)))
# define Z_BULKWRITE_OBJ_P(zv)         (php_bulkwrite_fetch_object(Z_OBJ_P(zv)))
# define Z_WRITECONCERN_OBJ_P(zv)      (php_writeconcern_fetch_object(Z_OBJ_P(zv)))
# define Z_WRITECONCERNERROR_OBJ_P(zv) (php_writeconcernerror_fetch_object(Z_OBJ_P(zv)))
# define Z_WRITEERROR_OBJ_P(zv)        (php_writeerror_fetch_object(Z_OBJ_P(zv)))
# define Z_WRITERESULT_OBJ_P(zv)       (php_writeresult_fetch_object(Z_OBJ_P(zv)))
# define Z_BINARY_OBJ_P(zv)            (php_binary_fetch_object(Z_OBJ_P(zv)))
# define Z_DECIMAL128_OBJ_P(zv)        (php_decimal128_fetch_object(Z_OBJ_P(zv)))
# define Z_JAVASCRIPT_OBJ_P(zv)        (php_javascript_fetch_object(Z_OBJ_P(zv)))
# define Z_MAXKEY_OBJ_P(zv)            (php_maxkey_fetch_object(Z_OBJ_P(zv)))
# define Z_MINKEY_OBJ_P(zv)            (php_minkey_fetch_object(Z_OBJ_P(zv)))
# define Z_OBJECTID_OBJ_P(zv)          (php_objectid_fetch_object(Z_OBJ_P(zv)))
# define Z_REGEX_OBJ_P(zv)             (php_regex_fetch_object(Z_OBJ_P(zv)))
# define Z_TIMESTAMP_OBJ_P(zv)         (php_timestamp_fetch_object(Z_OBJ_P(zv)))
# define Z_UTCDATETIME_OBJ_P(zv)       (php_utcdatetime_fetch_object(Z_OBJ_P(zv)))

# define Z_OBJ_COMMAND(zo)             (php_command_fetch_object(zo))
# define Z_OBJ_CURSOR(zo)              (php_cursor_fetch_object(zo))
# define Z_OBJ_CURSORID(zo)            (php_cursorid_fetch_object(zo))
# define Z_OBJ_MANAGER(zo)             (php_manager_fetch_object(zo))
# define Z_OBJ_QUERY(zo)               (php_query_fetch_object(zo))
# define Z_OBJ_READCONCERN(zo)         (php_readconcern_fetch_object(zo))
# define Z_OBJ_READPREFERENCE(zo)      (php_readpreference_fetch_object(zo))
# define Z_OBJ_SERVER(zo)              (php_server_fetch_object(zo))
# define Z_OBJ_BULKWRITE(zo)           (php_bulkwrite_fetch_object(zo))
# define Z_OBJ_WRITECONCERN(zo)        (php_writeconcern_fetch_object(zo))
# define Z_OBJ_WRITECONCERNERROR(zo)   (php_writeconcernerror_fetch_object(zo))
# define Z_OBJ_WRITEERROR(zo)          (php_writeerror_fetch_object(zo))
# define Z_OBJ_WRITERESULT(zo)         (php_writeresult_fetch_object(zo))
# define Z_OBJ_BINARY(zo)              (php_binary_fetch_object(zo))
# define Z_OBJ_DECIMAL128(zo)          (php_decimal128_fetch_object(zo))
# define Z_OBJ_JAVASCRIPT(zo)          (php_javascript_fetch_object(zo))
# define Z_OBJ_MAXKEY(zo)              (php_maxkey_fetch_object(zo))
# define Z_OBJ_MINKEY(zo)              (php_minkey_fetch_object(zo))
# define Z_OBJ_OBJECTID(zo)            (php_objectid_fetch_object(zo))
# define Z_OBJ_REGEX(zo)               (php_regex_fetch_object(zo))
# define Z_OBJ_TIMESTAMP(zo)           (php_timestamp_fetch_object(zo))
# define Z_OBJ_UTCDATETIME(zo)         (php_utcdatetime_fetch_object(zo))

#else

# define Z_COMMAND_OBJ_P(zv)           ((php_phongo_command_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_CURSOR_OBJ_P(zv)            ((php_phongo_cursor_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_CURSORID_OBJ_P(zv)          ((php_phongo_cursorid_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_MANAGER_OBJ_P(zv)           ((php_phongo_manager_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_QUERY_OBJ_P(zv)             ((php_phongo_query_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_READCONCERN_OBJ_P(zv)       ((php_phongo_readconcern_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_READPREFERENCE_OBJ_P(zv)    ((php_phongo_readpreference_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_SERVER_OBJ_P(zv)            ((php_phongo_server_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_BULKWRITE_OBJ_P(zv)         ((php_phongo_bulkwrite_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_WRITECONCERN_OBJ_P(zv)      ((php_phongo_writeconcern_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_WRITECONCERNERROR_OBJ_P(zv) ((php_phongo_writeconcernerror_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_WRITEERROR_OBJ_P(zv)        ((php_phongo_writeerror_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_WRITERESULT_OBJ_P(zv)       ((php_phongo_writeresult_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_BINARY_OBJ_P(zv)            ((php_phongo_binary_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_DECIMAL128_OBJ_P(zv)        ((php_phongo_decimal128_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_JAVASCRIPT_OBJ_P(zv)        ((php_phongo_javascript_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_MAXKEY_OBJ_P(zv)            ((php_phongo_maxkey_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_MINKEY_OBJ_P(zv)            ((php_phongo_minkey_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_OBJECTID_OBJ_P(zv)          ((php_phongo_objectid_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_REGEX_OBJ_P(zv)             ((php_phongo_regex_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_TIMESTAMP_OBJ_P(zv)         ((php_phongo_timestamp_t *)zend_object_store_get_object(zv TSRMLS_CC))
# define Z_UTCDATETIME_OBJ_P(zv)       ((php_phongo_utcdatetime_t *)zend_object_store_get_object(zv TSRMLS_CC))

# define Z_OBJ_COMMAND(zo)             ((php_phongo_command_t *)zo)
# define Z_OBJ_CURSOR(zo)              ((php_phongo_cursor_t *)zo)
# define Z_OBJ_CURSORID(zo)            ((php_phongo_cursorid_t *)zo)
# define Z_OBJ_MANAGER(zo)             ((php_phongo_manager_t *)zo)
# define Z_OBJ_QUERY(zo)               ((php_phongo_query_t *)zo)
# define Z_OBJ_READCONCERN(zo)         ((php_phongo_readconcern_t *)zo)
# define Z_OBJ_READPREFERENCE(zo)      ((php_phongo_readpreference_t *)zo)
# define Z_OBJ_SERVER(zo)              ((php_phongo_server_t *)zo)
# define Z_OBJ_BULKWRITE(zo)           ((php_phongo_bulkwrite_t *)zo)
# define Z_OBJ_WRITECONCERN(zo)        ((php_phongo_writeconcern_t *)zo)
# define Z_OBJ_WRITECONCERNERROR(zo)   ((php_phongo_writeconcernerror_t *)zo)
# define Z_OBJ_WRITEERROR(zo)          ((php_phongo_writeerror_t *)zo)
# define Z_OBJ_WRITERESULT(zo)         ((php_phongo_writeresult_t *)zo)
# define Z_OBJ_BINARY(zo)              ((php_phongo_binary_t *)zo)
# define Z_OBJ_DECIMAL128(zo)          ((php_phongo_decimal128_t *)zo)
# define Z_OBJ_JAVASCRIPT(zo)          ((php_phongo_javascript_t *)zo)
# define Z_OBJ_MAXKEY(zo)              ((php_phongo_maxkey_t *)zo)
# define Z_OBJ_MINKEY(zo)              ((php_phongo_minkey_t *)zo)
# define Z_OBJ_OBJECTID(zo)            ((php_phongo_objectid_t *)zo)
# define Z_OBJ_REGEX(zo)               ((php_phongo_regex_t *)zo)
# define Z_OBJ_TIMESTAMP(zo)           ((php_phongo_timestamp_t *)zo)
# define Z_OBJ_UTCDATETIME(zo)         ((php_phongo_utcdatetime_t *)zo)

#endif

typedef struct {
	zend_object_iterator   intern;
	php_phongo_cursor_t   *cursor;
} php_phongo_cursor_iterator;


extern PHONGO_API zend_class_entry *php_phongo_command_ce;
extern PHONGO_API zend_class_entry *php_phongo_cursor_ce;
extern PHONGO_API zend_class_entry *php_phongo_cursorid_ce;
extern PHONGO_API zend_class_entry *php_phongo_manager_ce;
extern PHONGO_API zend_class_entry *php_phongo_query_ce;
extern PHONGO_API zend_class_entry *php_phongo_readconcern_ce;
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
extern PHONGO_API zend_class_entry *php_phongo_decimal128_ce;
extern PHONGO_API zend_class_entry *php_phongo_javascript_ce;
extern PHONGO_API zend_class_entry *php_phongo_maxkey_ce;
extern PHONGO_API zend_class_entry *php_phongo_minkey_ce;
extern PHONGO_API zend_class_entry *php_phongo_objectid_ce;
extern PHONGO_API zend_class_entry *php_phongo_regex_ce;
extern PHONGO_API zend_class_entry *php_phongo_timestamp_ce;
extern PHONGO_API zend_class_entry *php_phongo_utcdatetime_ce;

/* Shared across all MongoDB\Driver objects to disable unserialize() */
PHP_METHOD(Manager, __wakeup);
/* Shared across all final MongoDB\Driver value objects, only possible to construct them internally */
PHP_METHOD(Server, __construct);

PHP_MINIT_FUNCTION(Command);
PHP_MINIT_FUNCTION(Cursor);
PHP_MINIT_FUNCTION(CursorId);
PHP_MINIT_FUNCTION(Manager);
PHP_MINIT_FUNCTION(Query);
PHP_MINIT_FUNCTION(ReadConcern);
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
PHP_MINIT_FUNCTION(Decimal128);
PHP_MINIT_FUNCTION(Javascript);
PHP_MINIT_FUNCTION(MaxKey);
PHP_MINIT_FUNCTION(MinKey);
PHP_MINIT_FUNCTION(ObjectID);
PHP_MINIT_FUNCTION(Regex);
PHP_MINIT_FUNCTION(Timestamp);
PHP_MINIT_FUNCTION(UTCDateTime);

#endif /* PHONGO_CLASSES_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

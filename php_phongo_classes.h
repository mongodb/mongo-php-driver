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

#ifndef PHONGO_CLASSES_H
#define PHONGO_CLASSES_H

#if PHP_VERSION_ID >= 70000
# include "php_phongo_structs-7.h"
# define Z_COMMAND_OBJ_P(zv)           php_command_fetch_object(Z_OBJ_P(zv));
# define Z_CURSOR_OBJ_P(zv)            php_cursor_fetch_object(Z_OBJ_P(zv));
# define Z_CURSORID_OBJ_P(zv)          php_cursorid_fetch_object(Z_OBJ_P(zv));
# define Z_MANAGER_OBJ_P(zv)           php_manager_fetch_object(Z_OBJ_P(zv));
# define Z_QUERY_OBJ_P(zv)             php_query_fetch_object(Z_OBJ_P(zv));
# define Z_READPREFERENCE_OBJ_P(zv)    php_readpreference_fetch_object(Z_OBJ_P(zv));
# define Z_SERVER_OBJ_P(zv)            php_server_fetch_object(Z_OBJ_P(zv));
# define Z_BULKWRITE_OBJ_P(zv)         php_bulkwrite_fetch_object(Z_OBJ_P(zv));
# define Z_WRITECONCERN_OBJ_P(zv)      php_writeconcern_fetch_object(Z_OBJ_P(zv));
# define Z_WRITECONCERNERROR_OBJ_P(zv) php_writeconcernerror_fetch_object(Z_OBJ_P(zv));
# define Z_WRITEERROR_OBJ_P(zv)        php_writeerror_fetch_object(Z_OBJ_P(zv));
# define Z_WRITERESULT_OBJ_P(zv)       php_writeresult_fetch_object(Z_OBJ_P(zv));
# define Z_BINARY_OBJ_P(zv)            php_binary_fetch_object(Z_OBJ_P(zv));
# define Z_INT32_OBJ_P(zv)             php_int32_fetch_object(Z_OBJ_P(zv));
# define Z_INT64_OBJ_P(zv)             php_int64_fetch_object(Z_OBJ_P(zv));
# define Z_JAVASCRIPT_OBJ_P(zv)        php_javascript_fetch_object(Z_OBJ_P(zv));
# define Z_LOG_OBJ_P(zv)               php_log_fetch_object(Z_OBJ_P(zv));
# define Z_MAXKEY_OBJ_P(zv)            php_maxkey_fetch_object(Z_OBJ_P(zv));
# define Z_MINKEY_OBJ_P(zv)            php_minkey_fetch_object(Z_OBJ_P(zv));
# define Z_OBJECTID_OBJ_P(zv)          php_objectid_fetch_object(Z_OBJ_P(zv));
# define Z_REGEX_OBJ_P(zv)             php_regex_fetch_object(Z_OBJ_P(zv));
# define Z_TIMESTAMP_OBJ_P(zv)         php_timestamp_fetch_object(Z_OBJ_P(zv));
# define Z_UTCDATETIME_OBJ_P(zv)       php_utcdatetime_fetch_object(Z_OBJ_P(zv));

# define Z_COMMAND_OBJ(zo)           php_command_fetch_object((zo));
# define Z_CURSOR_OBJ(zo)            php_cursor_fetch_object((zo));
# define Z_CURSORID_OBJ(zo)          php_cursorid_fetch_object((zo));
# define Z_MANAGER_OBJ(zo)           php_manager_fetch_object((zo));
# define Z_QUERY_OBJ(zo)             php_query_fetch_object((zo));
# define Z_READPREFERENCE_OBJ(zo)    php_readpreference_fetch_object((zo));
# define Z_SERVER_OBJ(zo)            php_server_fetch_object((zo));
# define Z_BULKWRITE_OBJ(zo)         php_bulkwrite_fetch_object((zo));
# define Z_WRITECONCERN_OBJ(zo)      php_writeconcern_fetch_object((zo));
# define Z_WRITECONCERNERROR_OBJ(zo) php_writeconcernerror_fetch_object((zo));
# define Z_WRITEERROR_OBJ(zo)        php_writeerror_fetch_object((zo));
# define Z_WRITERESULT_OBJ(zo)       php_writeresult_fetch_object((zo));
# define Z_BINARY_OBJ(zo)            php_binary_fetch_object((zo));
# define Z_INT32_OBJ(zo)             php_int32_fetch_object((zo));
# define Z_INT64_OBJ(zo)             php_int64_fetch_object((zo));
# define Z_JAVASCRIPT_OBJ(zo)        php_javascript_fetch_object((zo));
# define Z_LOG_OBJ(zo)               php_log_fetch_object((zo));
# define Z_MAXKEY_OBJ(zo)            php_maxkey_fetch_object((zo));
# define Z_MINKEY_OBJ(zo)            php_minkey_fetch_object((zo));
# define Z_OBJECTID_OBJ(zo)          php_objectid_fetch_object((zo));
# define Z_REGEX_OBJ(zo)             php_regex_fetch_object((zo));
# define Z_TIMESTAMP_OBJ(zo)         php_timestamp_fetch_object((zo));
# define Z_UTCDATETIME_OBJ(zo)       php_utcdatetime_fetch_object((zo));
#else
# include "php_phongo_structs-5.h"
# define Z_COMMAND_OBJ_P(zv)           (php_phongo_command_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_CURSOR_OBJ_P(zv)            (php_phongo_cursor_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_CURSORID_OBJ_P(zv)          (php_phongo_cursorid_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_MANAGER_OBJ_P(zv)           (php_phongo_manager_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_QUERY_OBJ_P(zv)             (php_phongo_query_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_READPREFERENCE_OBJ_P(zv)    (php_phongo_readpreference_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_SERVER_OBJ_P(zv)            (php_phongo_server_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_BULKWRITE_OBJ_P(zv)         (php_phongo_bulkwrite_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_WRITECONCERN_OBJ_P(zv)      (php_phongo_writeconcern_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_WRITECONCERNERROR_OBJ_P(zv) (php_phongo_writeconcernerror_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_WRITEERROR_OBJ_P(zv)        (php_phongo_writeerror_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_WRITERESULT_OBJ_P(zv)       (php_phongo_writeresult_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_BINARY_OBJ_P(zv)            (php_phongo_binary_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_INT32_OBJ_P(zv)             (php_phongo_int32_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_INT64_OBJ_P(zv)             (php_phongo_int64_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_JAVASCRIPT_OBJ_P(zv)        (php_phongo_javascript_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_LOG_OBJ_P(zv)               (php_phongo_log_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_MAXKEY_OBJ_P(zv)            (php_phongo_maxkey_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_MINKEY_OBJ_P(zv)            (php_phongo_minkey_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_OBJECTID_OBJ_P(zv)          (php_phongo_objectid_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_REGEX_OBJ_P(zv)             (php_phongo_regex_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_TIMESTAMP_OBJ_P(zv)         (php_phongo_timestamp_t *)zend_object_store_get_object(zv TSRMLS_CC);
# define Z_UTCDATETIME_OBJ_P(zv)       (php_phongo_utcdatetime_t *)zend_object_store_get_object(zv TSRMLS_CC);

# define Z_COMMAND_OBJ(zo)           (php_phongo_command_t *)(zo);
# define Z_CURSOR_OBJ(zo)            (php_phongo_cursor_t *)(zo);
# define Z_CURSORID_OBJ(zo)          (php_phongo_cursorid_t *)(zo);
# define Z_MANAGER_OBJ(zo)           (php_phongo_manager_t *)(zo);
# define Z_QUERY_OBJ(zo)             (php_phongo_query_t *)(zo);
# define Z_READPREFERENCE_OBJ(zo)    (php_phongo_readpreference_t *)(zo);
# define Z_SERVER_OBJ(zo)            (php_phongo_server_t *)(zo);
# define Z_BULKWRITE_OBJ(zo)         (php_phongo_bulkwrite_t *)(zo);
# define Z_WRITECONCERN_OBJ(zo)      (php_phongo_writeconcern_t *)(zo);
# define Z_WRITECONCERNERROR_OBJ(zo) (php_phongo_writeconcernerror_t *)(zo);
# define Z_WRITEERROR_OBJ(zo)        (php_phongo_writeerror_t *)(zo);
# define Z_WRITERESULT_OBJ(zo)       (php_phongo_writeresult_t *)(zo);
# define Z_BINARY_OBJ(zo)            (php_phongo_binary_t *)(zo);
# define Z_INT32_OBJ(zo)             (php_phongo_int32_t *)(zo);
# define Z_INT64_OBJ(zo)             (php_phongo_int64_t *)(zo);
# define Z_JAVASCRIPT_OBJ(zo)        (php_phongo_javascript_t *)(zo);
# define Z_LOG_OBJ(zo)               (php_phongo_log_t *)(zo);
# define Z_MAXKEY_OBJ(zo)            (php_phongo_maxkey_t *)(zo);
# define Z_MINKEY_OBJ(zo)            (php_phongo_minkey_t *)(zo);
# define Z_OBJECTID_OBJ(zo)          (php_phongo_objectid_t *)(zo);
# define Z_REGEX_OBJ(zo)             (php_phongo_regex_t *)(zo);
# define Z_TIMESTAMP_OBJ(zo)         (php_phongo_timestamp_t *)(zo);
# define Z_UTCDATETIME_OBJ(zo)       (php_phongo_utcdatetime_t *)(zo);
#endif

typedef struct {
	zend_object_iterator   intern;
	php_phongo_cursor_t   *cursor;
	long                   current;
} php_phongo_cursor_iterator;


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

#endif /* PHONGO_CLASSES_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

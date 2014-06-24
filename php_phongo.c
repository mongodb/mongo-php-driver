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

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "ext/spl/spl_iterators.h"
#include "ext/spl/spl_exceptions.h"
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain)
{
	switch (domain) {
		case PHONGO_INVALID_ARGUMENT:
			return spl_ce_InvalidArgumentException;
		case PHONGO_RUNETIME_ERROR:
			return spl_ce_RuntimeException;
	}
}
zend_class_entry* phongo_exception_from_mongoc_domain(mongoc_error_domain_t domain)
{
	switch (domain) {
		case MONGOC_ERROR_STREAM:
			/* FIXME: We don't have the Exceptions mocked yet.. */
#if 0
			return phongo_ce_mongo_connection_exception;
#endif
		default:
			return spl_ce_RuntimeException;
	}
}
PHONGO_API void phongo_throw_exception(php_phongo_error_domain_t domain TSRMLS_DC, char *message)
{
	zend_throw_exception(phongo_exception_from_phongo_domain(domain) TSRMLS_CC, message, 0);
}
/* }}} */


/* {{{ M[INIT|SHUTDOWN] R[INIT|SHUTDOWN] G[INIT|SHUTDOWN] MINFO INI */

ZEND_DECLARE_MODULE_GLOBALS(phongo)
/* {{{ INI entries */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY((char *)"phongo.be_awesome", (char *)"yes", PHP_INI_ALL, OnUpdateString, be_awesome, zend_phongo_globals, phongo_globals)
PHP_INI_END()
/* }}} */

/* {{{ PHP_GINIT_FUNCTION */
PHP_GINIT_FUNCTION(phongo)
{
	phongo_globals->be_awesome = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */

	REGISTER_INI_ENTRIES();

	/* Initialize libmongoc */
	mongoc_init();

	/* Prep default object handlers to be used when we register the classes */
	/*
	memcpy(&phongo_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_object_handlers.clone_obj = NULL;
	phongo_object_handlers.read_property = NULL;
	phongo_object_handlers.write_property = NULL;
	phongo_object_handlers.get_debug_info = NULL;
	*/

	PHP_MINIT(bson)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(CursorId)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(GeneratedId)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Cursor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(CommandCursor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(QueryCursor)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(WriteResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(DeleteResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(InsertResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UpdateResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(CommandResult)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(WriteBatch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(DeleteBatch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(InsertBatch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UpdateBatch)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Manager)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Server)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Command)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Query)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(ReadPreference)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(WriteConcernError)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteError)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* Really doesn't matter which module number we are */

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* Really doesn't matter which module number we are */

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */

	/* Cleanup after libmongoc */
	mongoc_cleanup();

	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION */
PHP_GSHUTDOWN_FUNCTION(phongo)
{
	phongo_globals->be_awesome = NULL;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(phongo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phongo support", "enabled");
	php_info_print_table_row(2, "phongo", PHONGO_VERSION_S);
	php_info_print_table_row(2, "libmongoc", MONGOC_VERSION_S);
	php_info_print_table_row(2, "libbson", BSON_VERSION_S);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */
/* }}} */

/* {{{ phongo_functions[]
*/
const zend_function_entry phongo_functions[] = {
	PHP_FE(bson_encode,     NULL)
	PHP_FE(bson_decode,     NULL)
	PHP_FE(bson_to_json,    NULL)
	PHP_FE_END
};
/* }}} */
/* {{{ phongo_module_entry
 */
zend_module_entry phongo_module_entry = {
	STANDARD_MODULE_HEADER,
	"phongo",
	phongo_functions,
	PHP_MINIT(phongo),
	PHP_MSHUTDOWN(phongo),
	PHP_RINIT(phongo),
	PHP_RSHUTDOWN(phongo),
	PHP_MINFO(phongo),
	PHONGO_VERSION,
	PHP_MODULE_GLOBALS(phongo),
	PHP_GINIT(phongo),
	PHP_GSHUTDOWN(phongo),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_PHONGO
ZEND_GET_MODULE(phongo)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

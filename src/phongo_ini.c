/*
 * Copyright 2021-present MongoDB, Inc.
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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>
#include <main/php_open_temporary_file.h>
#include <ext/date/php_date.h>

#include "php_phongo.h"
#include "phongo_ini.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

static void phongo_log(mongoc_log_level_t log_level, const char* log_domain, const char* message, void* user_data)
{
	struct timeval tv;
	time_t         t;
	zend_long      tu;
	zend_string*   dt;

	bson_gettimeofday(&tv);
	t  = tv.tv_sec;
	tu = tv.tv_usec;

	dt = php_format_date((char*) ZEND_STRL("Y-m-d\\TH:i:s"), t, 0);

	fprintf(MONGODB_G(debug_fd), "[%s.%06" PHONGO_LONG_FORMAT "+00:00] %10s: %-8s> %s\n", ZSTR_VAL(dt), tu, log_domain, mongoc_log_level_str(log_level), message);
	fflush(MONGODB_G(debug_fd));
	efree(dt);
}

void phongo_log_disable(FILE* stream)
{
	mongoc_log_trace_disable();
	mongoc_log_set_handler(NULL, NULL);

	/* Close any previously opened log file (excluding stderr/stdout) */
	if (stream && stream != stderr && stream != stdout) {
		fclose(stream);
	}
}

static PHP_INI_MH(OnUpdateDebug)
{
	char* tmp_dir = NULL;

	phongo_log_disable(MONGODB_G(debug_fd));
	MONGODB_G(debug_fd) = NULL;

	if (!new_value || (new_value && !ZSTR_VAL(new_value)[0]) || strcasecmp("0", ZSTR_VAL(new_value)) == 0 || strcasecmp("off", ZSTR_VAL(new_value)) == 0 || strcasecmp("no", ZSTR_VAL(new_value)) == 0 || strcasecmp("false", ZSTR_VAL(new_value)) == 0) {
		return OnUpdateString(entry, new_value, mh_arg1, mh_arg2, mh_arg3, stage);
	}

	if (strcasecmp(ZSTR_VAL(new_value), "stderr") == 0) {
		MONGODB_G(debug_fd) = stderr;
	} else if (strcasecmp(ZSTR_VAL(new_value), "stdout") == 0) {
		MONGODB_G(debug_fd) = stdout;
	} else if (
		strcasecmp("1", ZSTR_VAL(new_value)) == 0 ||
		strcasecmp("on", ZSTR_VAL(new_value)) == 0 ||
		strcasecmp("yes", ZSTR_VAL(new_value)) == 0 ||
		strcasecmp("true", ZSTR_VAL(new_value)) == 0) {

		tmp_dir = NULL;
	} else {
		tmp_dir = ZSTR_VAL(new_value);
	}

	if (!MONGODB_G(debug_fd)) {
		time_t       t;
		int          fd = -1;
		char*        prefix;
		int          len;
		zend_string* filename;

		time(&t);
		len = spprintf(&prefix, 0, "PHONGO-%ld", t);

		fd = php_open_temporary_fd(tmp_dir, prefix, &filename);
		if (fd != -1) {
			const char* path    = ZSTR_VAL(filename);
			MONGODB_G(debug_fd) = VCWD_FOPEN(path, "a");
		}
		efree(filename);
		efree(prefix);
		close(fd);
	}

	mongoc_log_trace_enable();
	mongoc_log_set_handler(phongo_log, NULL);

	return OnUpdateString(entry, new_value, mh_arg1, mh_arg2, mh_arg3, stage);
}

void phongo_display_ini_entries(ZEND_MODULE_INFO_FUNC_ARGS)
{
	DISPLAY_INI_ENTRIES();
}

void phongo_register_ini_entries(INIT_FUNC_ARGS)
{
	PHP_INI_BEGIN()
		STD_PHP_INI_ENTRY("mongodb.debug", "", PHP_INI_ALL, OnUpdateDebug, debug, zend_mongodb_globals, mongodb_globals)
	PHP_INI_END()

	REGISTER_INI_ENTRIES();
}

void phongo_unregister_ini_entries(SHUTDOWN_FUNC_ARGS)
{
	UNREGISTER_INI_ENTRIES();
}

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

#include <stdio.h>
#include <time.h>

#include <php.h>
#include <main/php_open_temporary_file.h>

#include "php_phongo.h"
#include "phongo_ini.h"
#include "phongo_log.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

static FILE* phongo_ini_tmp_file(const char* tmp_dir)
{
	int          fd = -1;
	char*        prefix;
	int          len;
	zend_string* filename = NULL;
	FILE*        stream   = NULL;

	len = spprintf(&prefix, 0, "PHONGO-%ld", time(NULL));

	/* TODO: Refactor this to use fdopen (see: PHPC-2181) */
	fd = php_open_temporary_fd(tmp_dir, prefix, &filename);

	if (fd != -1) {
		close(fd);
	}

	if (filename) {
		stream = VCWD_FOPEN(ZSTR_VAL(filename), "a");
		efree(filename);
	}

	efree(prefix);

	return stream;
}

static PHP_INI_MH(OnUpdateDebug)
{
	FILE* stream = NULL;

	if (!new_value ||
		zend_string_equals_literal_ci(new_value, "") ||
		zend_string_equals_literal_ci(new_value, "0") ||
		zend_string_equals_literal_ci(new_value, "off") ||
		zend_string_equals_literal_ci(new_value, "no") ||
		zend_string_equals_literal_ci(new_value, "false")) {

		goto done;
	}

	if (zend_string_equals_literal_ci(new_value, "stderr")) {
		stream = stderr;
	} else if (zend_string_equals_literal_ci(new_value, "stdout")) {
		stream = stdout;
	} else if (
		zend_string_equals_literal_ci(new_value, "1") ||
		zend_string_equals_literal_ci(new_value, "on") ||
		zend_string_equals_literal_ci(new_value, "yes") ||
		zend_string_equals_literal_ci(new_value, "true")) {

		stream = phongo_ini_tmp_file(NULL);
	} else {
		stream = phongo_ini_tmp_file(ZSTR_VAL(new_value));
	}

	/* TODO: Consider failing if we do not have a stream by this point */

done:
	phongo_log_set_stream(stream);

	/* OnUpdateString should always succeed, but defer to its retval anyway */
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

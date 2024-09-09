/*
 * Copyright 2014-present MongoDB, Inc.
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

#include "php_phongo.h"
#include "phongo_error.h"
#include "WriteException_arginfo.h"

zend_class_entry* php_phongo_writeexception_ce;

/* Returns the WriteResult from the failed write operation. */
static PHP_METHOD(MongoDB_Driver_Exception_WriteException, getWriteResult)
{
	zval* writeresult;
	zval  rv;

	PHONGO_PARSE_PARAMETERS_NONE();

	writeresult = zend_read_property(php_phongo_writeexception_ce, Z_OBJ_P(getThis()), ZEND_STRL("writeResult"), 0, &rv);

	RETURN_ZVAL(writeresult, 1, 0);
}

void php_phongo_writeexception_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_writeexception_ce = register_class_MongoDB_Driver_Exception_WriteException(php_phongo_serverexception_ce);
}

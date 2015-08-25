/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2015 MongoDB, Inc.                                         |
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
  | Copyright (c) 2015 MongoDB, Inc.                                          |
  +---------------------------------------------------------------------------+
*/


/* Our Compatability header */
#include "phongo_compat.h"


void *x509_from_zval(zval *zval TSRMLS_DC) {
#if PHP_VERSION_ID >= 70000
	return Z_RES_P(zval)->ptr;
#else
	int resource_type;
	int type;

	zend_list_find(Z_LVAL_P(zval), &resource_type);
	return zend_fetch_resource(&zval TSRMLS_CC, -1, "OpenSSL X.509", &type, 1, resource_type);
#endif
}

void phongo_add_exception_prop(const char *prop, int prop_len, zval *value TSRMLS_DC)
{
	if (EG(exception)) {
#if PHP_VERSION_ID >= 70000
		zval ex;
		ZVAL_OBJ(&ex, EG(exception));
		zend_update_property(Z_OBJCE(ex), &ex, prop, prop_len, value);
#else
		zval *ex = EG(exception);
		zend_update_property(Z_OBJCE_P(ex), ex, prop, prop_len, value TSRMLS_CC);
#endif

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

/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2014 Hannes Magnusson                                  |
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

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>

/* PHP Core stuff */
#include <php.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


#define BSON_APPEND_INT32(b,key,val) \
	bson_append_int32 (b, key, (int) strlen (key), val)

#if SIZEOF_LONG == 8
#	define BSON_APPEND_INT(b, key, keylen, val) \
	bson_append_int64(b, key, keylen, val);
#elif SIZEOF_LONG == 4
#	define BSON_APPEND_INT(b, key, keylen, val) \
	bson_append_int32(b, key, keylen, val);
#endif

PHP_MINIT_FUNCTION(bson)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* Really doesn't matter which module number we are */

	/* Register BSONable */
	return SUCCESS;
}

/* Forward declarations */
bool php_phongo_bson_visit_document(const bson_iter_t *iter __attribute__((unused)), const char *key, const bson_t *v_document, void *data);
bool php_phongo_bson_visit_array(const bson_iter_t *iter __attribute__((unused)), const char *key, const bson_t *v_document, void *data);

#if 0
bool php_phongo_bson_visit_before(const bson_iter_t *iter __attribute__((unused)), const char *key, void *data) /* {{{ */
{
	return false;
}
/* }}} */
bool php_phongo_bson_visit_after(const bson_iter_t *iter __attribute__((unused)), const char *key, void *data) /* {{{ */
{
	return false;
}
/* }}} */
#endif
void php_phongo_bson_visit_corrupt(const bson_iter_t *iter __attribute__((unused)), void *data) /* {{{ */
{
	zval *retval = *(zval **)data;

	zval_ptr_dtor(&retval);
}
/* }}} */
bool php_phongo_bson_visit_double(const bson_iter_t *iter __attribute__((unused)), const char *key, double v_double, void *data) /* {{{ */
{
	zval *retval = *(zval **)data;
	TSRMLS_FETCH();

	if (Z_TYPE_P(retval) == IS_ARRAY) {
		add_assoc_double(retval, key, v_double);
	} else if (Z_TYPE_P(retval) == IS_OBJECT) {
		add_property_double(retval, key, v_double);
	}
	return false;
}
/* }}} */
bool php_phongo_bson_visit_utf8(const bson_iter_t *iter __attribute__((unused)), const char *key, size_t v_utf8_len, const char *v_utf8, void *data) /* {{{ */
{
	zval *retval = *(zval **)data;
	TSRMLS_FETCH();

	if (Z_TYPE_P(retval) == IS_ARRAY) {
		add_assoc_stringl(retval, key, (char *)v_utf8, v_utf8_len, 1);
	} else if (Z_TYPE_P(retval) == IS_OBJECT) {
		add_property_stringl(retval, key, v_utf8, v_utf8_len, 1);
	} else {
		return true;
	}

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_binary(const bson_iter_t *iter __attribute__((unused)), const char *key, bson_subtype_t v_subtype, size_t v_binary_len, const uint8_t *v_binary, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
bool php_phongo_bson_visit_undefined(const bson_iter_t *iter __attribute__((unused)), const char *key, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
bool php_phongo_bson_visit_oid(const bson_iter_t *iter __attribute__((unused)), const char *key, const bson_oid_t *v_oid, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif
bool php_phongo_bson_visit_bool(const bson_iter_t *iter __attribute__((unused)), const char *key, bool v_bool, void *data) /* {{{ */
{
	zval *retval = *(zval **)data;
	TSRMLS_FETCH();

	if (Z_TYPE_P(retval) == IS_ARRAY) {
		add_assoc_bool(retval, key, v_bool);
	} else if (Z_TYPE_P(retval) == IS_OBJECT) {
		add_property_bool(retval, key, v_bool);
	} else {
		return true;
	}

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_date_time(const bson_iter_t *iter __attribute__((unused)), const char *key, int64_t msec_since_epoch, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif
bool php_phongo_bson_visit_null(const bson_iter_t *iter __attribute__((unused)), const char *key, void *data) /* {{{ */
{
	zval *retval = *(zval **)data;
	TSRMLS_FETCH();

	if (Z_TYPE_P(retval) == IS_ARRAY) {
		add_assoc_null(retval, key);
	} else if (Z_TYPE_P(retval) == IS_OBJECT) {
		add_property_null(retval, key);
	} else {
		return true;
	}

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_regex(const bson_iter_t *iter __attribute__((unused)), const char *key, const char *v_regex, const char *v_options, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
bool php_phongo_bson_visit_dbpointer(const bson_iter_t *iter __attribute__((unused)), const char *key, size_t v_collection_len, const char *v_collection, const bson_oid_t *v_oid, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
bool php_phongo_bson_visit_code(const bson_iter_t *iter __attribute__((unused)), const char *key, size_t v_code_len, const char *v_code, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
bool php_phongo_bson_visit_symbol(const bson_iter_t *iter __attribute__((unused)), const char *key, size_t v_symbol_len, const char *v_symbol, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
bool php_phongo_bson_visit_codewscope(const bson_iter_t *iter __attribute__((unused)), const char *key, size_t v_code_len, const char *v_code, const bson_t *v_scope, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif
bool php_phongo_bson_visit_int32(const bson_iter_t *iter __attribute__((unused)), const char *key, int32_t v_int32, void *data) /* {{{ */
{
	zval *retval = *(zval **)data;
	TSRMLS_FETCH();

	if (Z_TYPE_P(retval) == IS_ARRAY) {
		add_assoc_long(retval, key, v_int32);
	} else if (Z_TYPE_P(retval) == IS_OBJECT) {
		add_property_long(retval, key, v_int32);
	} else {
		return true;
	}

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_timestamp(const bson_iter_t *iter __attribute__((unused)), const char *key, uint32_t v_timestamp, uint32_t v_increment, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif
bool php_phongo_bson_visit_int64(const bson_iter_t *iter __attribute__((unused)), const char *key, int64_t v_int64, void *data) /* {{{ */
{
	zval *retval = *(zval **)data;
	TSRMLS_FETCH();

	if (Z_TYPE_P(retval) == IS_ARRAY) {
		add_assoc_long(retval, key, v_int64);
	} else if (Z_TYPE_P(retval) == IS_OBJECT) {
		add_property_long(retval, key, v_int64);
	} else {
		return true;
	}

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_maxkey(const bson_iter_t *iter __attribute__((unused)), const char *key, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
bool php_phongo_bson_visit_minkey(const bson_iter_t *iter __attribute__((unused)), const char *key, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif


static const bson_visitor_t php_bson_visitors = {
   NULL /* php_phongo_bson_visit_before*/,
   NULL /*php_phongo_bson_visit_after*/,
   NULL /*php_phongo_bson_visit_corrupt*/,
   php_phongo_bson_visit_double,
   php_phongo_bson_visit_utf8,
   php_phongo_bson_visit_document,
   php_phongo_bson_visit_array,
   NULL /*php_phongo_bson_visit_binary*/,
   NULL /*php_phongo_bson_visit_undefined*/,
   NULL /*php_phongo_bson_visit_oid*/,
   php_phongo_bson_visit_bool,
   NULL /*php_phongo_bson_visit_date_time*/,
   php_phongo_bson_visit_null,
   NULL /*php_phongo_bson_visit_regex*/,
   NULL /*php_phongo_bson_visit_dbpointer*/,
   NULL /*php_phongo_bson_visit_code*/,
   NULL /*php_phongo_bson_visit_symbol*/,
   NULL /*php_phongo_bson_visit_codewscope*/,
   php_phongo_bson_visit_int32,
   NULL /*php_phongo_bson_visit_timestamp*/,
   php_phongo_bson_visit_int64,
   NULL /*php_phongo_bson_visit_maxkey*/,
   NULL /*php_phongo_bson_visit_minkey*/,
   { NULL }
};


bool php_phongo_bson_visit_document(const bson_iter_t *iter __attribute__((unused)), const char *key, const bson_t *v_document, void *data)
{
	zval *retval = *(zval **)data;
	bson_iter_t child;
	TSRMLS_FETCH();

	if (bson_iter_init(&child, v_document)) {
		zval *zchild;

		MAKE_STD_ZVAL(zchild);
		object_init(zchild);
		if (!bson_iter_visit_all(&child, &php_bson_visitors, &zchild)) {
			if (Z_TYPE_P(retval) == IS_ARRAY) {
				add_assoc_zval(retval, key, zchild);
			} else if (Z_TYPE_P(retval) == IS_OBJECT) {
				add_property_zval(retval, key, zchild);
			} else {
				return true;
			}
		}
	}

	return false;
}


bool php_phongo_bson_visit_array(const bson_iter_t *iter __attribute__((unused)), const char *key, const bson_t *v_array, void *data)
{
	zval *retval = *(zval **)data;
	bson_iter_t child;
	TSRMLS_FETCH();

	if (bson_iter_init(&child, v_array)) {
		zval *zchild;

		MAKE_STD_ZVAL(zchild);
		object_init(zchild);
		if (!bson_iter_visit_all(&child, &php_bson_visitors, &zchild)) {
			if (Z_TYPE_P(retval) == IS_ARRAY) {
				add_assoc_zval(retval, key, zchild);
			} else if (Z_TYPE_P(retval) == IS_OBJECT) {
				add_property_zval(retval, key, zchild);
				return true;
			}
		}
	}

	return false;
}


PHONGO_API int php_phongo_is_array_or_document(zval **val TSRMLS_DC) /* {{{ */
{
	HashTable *ht_data = HASH_OF(*val);
	int        count;

	if (Z_TYPE_PP(val) != IS_ARRAY) {
		return IS_OBJECT;
	}

	count = ht_data ? zend_hash_num_elements(ht_data) : 0;
	if (count > 0) {
		char         *key;
		uint          key_len;
		ulong         index;
		ulong         idx = 0;
		int           hash_type;
		HashPosition  pos;

		zend_hash_internal_pointer_reset_ex(ht_data, &pos);
		for (;; zend_hash_move_forward_ex(ht_data, &pos)) {
			hash_type = zend_hash_get_current_key_ex(ht_data, &key, &key_len, &index, 0, &pos);
			if (hash_type == HASH_KEY_NON_EXISTENT) {
				break;
			}

			if (hash_type == HASH_KEY_IS_STRING) {
				return IS_OBJECT;
			} else {
				if (index != idx) {
					return IS_OBJECT;
				}
			}
			idx++;
		}
	} else {
		return IS_OBJECT;
	}

	return IS_ARRAY;
}
/* }}} */
PHONGO_API void php_phongo_bson_encode_array(bson_t *bson, zval *data TSRMLS_DC) /* {{{ */
{
	HashTable *ht_data = HASH_OF(data);
	int        count;

	if (ht_data && ht_data->nApplyCount > 1) {
		return;
	}

	count = ht_data ? zend_hash_num_elements(ht_data) : 0;

	if (count > 0) {
		char         *key;
		uint          key_len;
		ulong         index;
		int           hash_type;
		HashPosition  pos;
		zval        **entry;

		zend_hash_internal_pointer_reset_ex(ht_data, &pos);
		for (;; zend_hash_move_forward_ex(ht_data, &pos)) {
			hash_type = zend_hash_get_current_key_ex(ht_data, &key, &key_len, &index, 0, &pos);
			if (hash_type == HASH_KEY_NON_EXISTENT) {
				break;
			}

			if (zend_hash_get_current_data_ex(ht_data, (void **) &entry, &pos) == SUCCESS) {
				HashTable *tmp_ht = HASH_OF(*entry);
				if (tmp_ht) {
					tmp_ht->nApplyCount++;
				}

				if (hash_type == HASH_KEY_IS_STRING) {
					php_phongo_bson_encode(bson, key, key_len-1, *entry TSRMLS_CC);
				} else {
					char *tmp_number;
					int tmp_number_len;
					char numbuf[32];

					tmp_number_len = bson_uint32_to_string(index, &tmp_number, numbuf, sizeof numbuf);
					php_phongo_bson_encode(bson, tmp_number, tmp_number_len, *entry TSRMLS_CC);
				}

				if (tmp_ht) {
					tmp_ht->nApplyCount--;
				}
			}
		}
	}

}

/* }}} */
PHONGO_API void php_phongo_bson_encode(bson_t *bson, char *key, int key_len, zval *val TSRMLS_DC) /* {{{ */
{
	switch (Z_TYPE_P(val))
	{
		case IS_NULL:
			bson_append_null(bson, key, key_len);
			break;

		case IS_BOOL:
			bson_append_bool(bson, key, key_len, Z_BVAL_P(val));
			break;

		case IS_LONG:
			BSON_APPEND_INT(bson, key, key_len, Z_LVAL_P(val));
			break;

		case IS_DOUBLE:
			bson_append_double(bson, key, key_len, Z_DVAL_P(val));
			break;

		case IS_STRING:
			if (bson_utf8_validate(Z_STRVAL_P(val), Z_STRLEN_P(val), true)) {
				bson_append_utf8(bson, key, key_len, Z_STRVAL_P(val), Z_STRLEN_P(val));
			} else {
				/* FIXME: Broken Broken Broken */
				printf("BROKEN BROKEN BROKEN UTF8\n");
			}
			break;

		case IS_OBJECT:
			/*
			if (instanceof_function(Z_OBJCE_P(val), php_bson_serializable_ce TSRMLS_CC)) {
				bson_encode_serializable_object(buf, val, TSRMLS_CC);
				break;
			}
			*/
			/* fallthrough -- Non-serializable object */
		case IS_ARRAY:
			{
				bson_t child;

				switch(php_phongo_is_array_or_document(&val TSRMLS_CC)) {
					case IS_OBJECT:
						bson_append_document_begin(bson, key, key_len, &child);
						php_phongo_bson_encode_array(&child, val TSRMLS_CC);
						bson_append_document_end(bson, &child);
						break;

					case IS_ARRAY:
						bson_append_array_begin(bson, key, key_len, &child);
						php_phongo_bson_encode_array(&child, val TSRMLS_CC);
						bson_append_array_end(bson, &child);
						break;
				}
			}
			break;

		default:
			break;
	}

	return;
}
/* }}} */
int bson_to_zval(const unsigned char *data, int data_len, zval *retval)
{
	      bson_reader_t *reader;
	      bson_iter_t    iter;
	const bson_t        *b;
	      bool           eof = false;
		  TSRMLS_FETCH();


	reader = bson_reader_new_from_data(data, data_len);

	if (!(b = bson_reader_read(reader, &eof))) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not read document from reader");
		return 0;
	}

	do {
		if (!bson_iter_init(&iter, b)) {
			bson_reader_destroy(reader);
			return 0;
		}

		array_init(retval);
		bson_iter_visit_all(&iter, &php_bson_visitors, &retval);
	} while ((b = bson_reader_read(reader, &eof)));

	bson_reader_destroy(reader);

	if (!eof) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid BSON detected");
		return 0;
	}

	return 1;
}

/* {{{ proto string bson_encode(mixed data)
   Returns the BSON representation of a value */
PHP_FUNCTION(bson_encode)
{
	zval   *data;
	bson_t *bson;

	(void)return_value_ptr; (void)this_ptr; (void)return_value_used; /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &data) == FAILURE) {
		return;
	}

	bson = bson_new();

	switch(Z_TYPE_P(data)) {
		case IS_OBJECT:
		case IS_ARRAY:
			php_phongo_bson_encode_array(bson, data TSRMLS_CC);
			break;

		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot convert scalar value '%s' to BSON", zend_zval_type_name(data));
			RETURN_NULL();
			break;
	}

	RETURN_STRINGL((const char *) bson_get_data(bson), bson->len, 1);
	bson_destroy(bson);
}
/* }}} */

/* {{{ proto string bson_decode(mixed data)
   Returns the PHP representation of a BSON value */
PHP_FUNCTION(bson_decode)
{
	      char          *data;
	      int            data_len;

	(void)return_value_ptr; (void)this_ptr; (void)return_value_used; /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		return;
	}

	if (!bson_to_zval((const unsigned char *)data, data_len, return_value)) {
		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto string bson_to_json(mixed data)
   Returns the JSON representation of a BSON value */
PHP_FUNCTION(bson_to_json)
{
	      char          *data;
	      int            data_len;
	const bson_t        *b;
	      bson_reader_t *reader;

	(void)return_value_ptr; (void)this_ptr; (void)return_value_used; /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		return;
	}

	reader = bson_reader_new_from_data((const unsigned char *)data, data_len);
	b = bson_reader_read(reader, NULL);

	if (b) {
		char   *str;
		size_t  str_len;
		str = bson_as_json(b, &str_len);
		RETVAL_STRINGL(str, str_len, 1);
		bson_free(str);
	} else {
		RETURN_NULL();
	}
	bson_reader_destroy(reader);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

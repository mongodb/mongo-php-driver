/* +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2018 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Sara Golemon (pollita@php.net)                               |
   +----------------------------------------------------------------------+
 */

#ifndef PHP_ARRAY_API_H
#define PHP_ARRAY_API_H

#include "zend.h"
#include "zend_execute.h"
#include "zend_API.h"
#include "zend_operators.h"
#include "zend_hash.h"
#include "zend_list.h"

#ifdef ZEND_ENGINE_3
# define PAA_LENGTH_ADJ(l) (l)
# define PAA_SYM_EXISTS zend_symtable_str_exists
# define PAA_SYM_DEL    zend_symtable_str_del
# define PAA_LONG       zend_long
# define PAA_ULONG      zend_ulong
#else
# define PAA_LENGTH_ADJ(l) (l+1)
# define PAA_SYM_EXISTS zend_symtable_exists
# define PAA_SYM_DEL    zend_symtable_del
# define PAA_LONG       long
# define PAA_ULONG      ulong
#endif

/**
 * All APIs in this file follow a general format:
 *
 * php_array_{$verb}{$modifier}_{$type}(zval *zarr, ...)
 *
 * $verb is one of:
 *   exists       - Boolean check whether the array offset exists
 *   fetch        - Retrieve the value at $zarr[$key]
 *   unset        - Delete the named offset from the array
 *
 * $modifier specifies what type of offset (key) is being used:
 *   <no modifer> - NULL terminated string variable, unknown length
 *   l            - NULL terminated string variable, known length
 *   l_safe       - String variable of known length, not necessarily NULL terminated
 *   n            - Long (integer) offset
 *   c            - NULL terminated string literal (e.g. "foo" rather than foo)
 *   z            - zval* offset, type should be NULL, BOOL, LONG, DOUBLE, or STRING
 *
 * $type is specific to the "fetch" verb:
 *   <no type>    - Fetch a zval* of any type
 *   bool         - Fetch a zend_bool         (converting as needed)
 *   long         - Fetch a long              (converting as needed)
 *   double       - Fetch a double            (converting as needed)
 *   string       - Fetch a string            (converting as needed, caller may need to free)
 *   array        - Fetch an array            (no conversion from other types)
 *   object       - Fetch an object           (no conversion, type spec optional)
 *   resource     - Fetch a resource          (no conversion, type spec mandatory)
 *
 * See the specific subsection for additional details
 */

/* isset($zarr[$key]) - Check for the existence of a key within an array
 *
 * zend_bool php_array_exists(zval *zarr, const char *key)
 * zend_bool php_array_existsc(zval *zarr, const char *litstr)
 * zend_bool php_array_existsl(zval *zarr, const char *key, int key_len)
 * zend_bool php_array_existsl_safe(zval *zarr, const char *key, int key_len)
 * zend_bool php_array_existsn(zval *zarr, unsigned long idx)
 * zend_bool php_array_existsz(zval *zarr, zval *key)
 */
static inline
zend_bool php_array_exists(zval *zarr, const char *key) {
	return PAA_SYM_EXISTS(Z_ARRVAL_P(zarr), key, PAA_LENGTH_ADJ(strlen(key)));
}
#define php_array_existsc(zarr, litstr) \
	PAA_SYM_EXISTS(Z_ARRVAL_P(zarr), litstr, PAA_LENGTH_ADJ(sizeof(litstr) - 1))
#define php_array_existsl(zarr, key, len) \
	PAA_SYM_EXISTS(Z_ARRVAL_P(zarr), key, PAA_LENGTH_ADJ(len))
static inline
zend_bool php_array_existsl_safe(zval *zarr, const char *key, int key_len) {
#ifdef ZEND_ENGINE_3
	zend_string *keystr = zend_string_init(key, key_len, 0);
	zend_bool ret = zend_symtable_exists(Z_ARRVAL_P(zarr), keystr);
	zend_string_release(keystr);
#else
	char *k = estrndup(key, key_len);
	zend_bool ret = zend_symtable_exists(Z_ARRVAL_P(zarr), k, key_len + 1);
	efree(k);
#endif
	return ret;
}
#define php_array_existsn(zarr, idx) \
	zend_hash_index_exists(Z_ARRVAL_P(zarr), idx) 
static inline
zend_bool php_array_existsz(zval *zarr, zval *key) {
	switch (Z_TYPE_P(key)) {
		case IS_NULL:
			return php_array_existsc(zarr, "");
#ifdef ZEND_ENGINE_3
		case IS_FALSE:
			return zend_hash_index_exists(Z_ARRVAL_P(zarr), 0);
		case IS_TRUE:
			return zend_hash_index_exists(Z_ARRVAL_P(zarr), 1);
#else
		case IS_BOOL: /* fallthrough */
#endif
		case IS_LONG:
			return zend_hash_index_exists(Z_ARRVAL_P(zarr), Z_LVAL_P(key));
		case IS_DOUBLE:
			return zend_hash_index_exists(Z_ARRVAL_P(zarr),
			                              zend_dval_to_lval(Z_DVAL_P(key)));
		case IS_STRING:
			return php_array_existsl(zarr, Z_STRVAL_P(key), Z_STRLEN_P(key));
		default:
			return 0;
	}
}

/* =$zarr[$key] - Fetch a zval (or appropriate type) from an array
 *
 * Methods returning pointers yield NULL on key not existing,
 * others yield 0, false, etc... as appropriate.
 * Callers needing to distinguish empty scalars from non-existent
 * scalars should use php_array_exists*() or fetch the zval then convert.
 *
 * If the type of the value does not match what is requested
 * it will be implicitly converted (if possible).
 *
 * See each type section for specific prototypes
 *
 * php_array_fetch*           - Fetch a zval
 * php_array_fetch*_bool      - Fetch a boolean
 * php_array_fetch*_long      - Fetch a long
 * php_array_fetch*_double    - Fetch a double
 * php_array_fetch*_string    - Fetch a string (must be efree()'d by caller)
 * php_array_fetch*_array     - Fetch an array
 * php_array_fetch*_resource  - Fetch a resource or a specific type
 * php_array_fetch*_object    - Fetch an object
 *
 * For each result type, there are six key forms:
 *   php_array_fetch_T(zval *zarr, const char *key, ...)
 *       NULL terminated string key
 *   php_array_fetchc_T(zval *zarr, const char *litkey, ...)
 *       String literal key
 *   php_array_fetchl_T(zval *zarr, const char *key, int key_len, ...)
 *       NULL terminated string key of known length
 *   php_array_fetchl_safe_T(zval *zarr, const char *key, int key_len, ...)
 *       String key of known length, may not be NULL terminated
 *   php_array_fetchn_T(zval *zarr, unsigned long idx, ...)
 *       Numeric key
 *   php_array_fetchz_T(zval *zarr, zval *key, ...)
 *       zval* key
 */

/* Fetch zval*
 *
 * zval *php_array_fetch(zval *zarr, const char *key)
 * zval *php_array_fetchl(zval *zarr, const char *key, int key_len)
 * zval *php_array_fetchl_safe(zval *zarr, const char *key, int key_len)
 * zval *php_array_fetchn(zval *zarr, unsigned long idx)
 * zval *php_array_fetchc(zval *zarr, const char *litstr)
 * zval *php_array_fetchz(zval *zarr, zval *key)
 */
static inline
zval *php_array_fetchl(zval *zarr, const char *key, int key_len) {
#ifdef ZEND_ENGINE_3
	return zend_symtable_str_find(Z_ARRVAL_P(zarr), key, key_len);
#else
	zval **ppzval;
	if (FAILURE == zend_symtable_find(Z_ARRVAL_P(zarr),
                                          key, key_len + 1,
                                          (void**)&ppzval)) {
		return NULL;
	}
	return *ppzval;
#endif
}
static inline
zval *php_array_fetch(zval *zarr, const char *key) {
	return php_array_fetchl(zarr, key, strlen(key));
}
#define php_array_fetchc(zarr, litstr) php_array_fetchl(zarr, litstr, sizeof(litstr)-1)
static inline
zval *php_array_fetchl_safe(zval *zarr, const char *key, int key_len) {
#ifdef ZEND_ENGINE_3
	zend_string *keystr = zend_string_init(key, key_len, 0);
	zval *ret = zend_symtable_find(Z_ARRVAL_P(zarr), keystr);
	zend_string_release(keystr);
#else
	char *k = estrndup(key, key_len);
	zval *ret = php_array_fetchl(zarr, k, key_len);
	efree(k);
#endif
	return ret;
}
static inline
zval *php_array_fetchn(zval *zarr, PAA_ULONG idx) {
#ifdef ZEND_ENGINE_3
	return zend_hash_index_find(Z_ARRVAL_P(zarr), idx);
#else
	zval **ppzval;
	if (FAILURE == zend_hash_index_find(Z_ARRVAL_P(zarr),
	                                    idx, (void**)&ppzval)) {
		return NULL;
	}
	return *ppzval;
#endif
}
static inline
zval *php_array_fetchz(zval *zarr, zval *key) {
	switch (Z_TYPE_P(key)) {
		case IS_NULL:
			return php_array_fetchn(zarr, 0);
#ifdef ZEND_ENGINE_3
		case IS_FALSE:
			return php_array_fetchn(zarr, 0);
		case IS_TRUE:
			return php_array_fetchn(zarr, 1);
#else
		case IS_BOOL: /* fallthrough */
#endif
		case IS_LONG:
			return php_array_fetchn(zarr, Z_LVAL_P(key));
		case IS_DOUBLE:
			return php_array_fetchn(zarr, (long)Z_DVAL_P(key));
		case IS_STRING:
			return php_array_fetchl(zarr, Z_STRVAL_P(key), Z_STRLEN_P(key));
		default:
			return NULL;
	}
}

#define PHP_ARRAY_FETCH_TYPE_MAP(ctype, ztype) \
static inline ctype php_array_fetch_##ztype(zval *zarr, const char *key) \
  	{ return php_array_zval_to_##ztype(php_array_fetch(zarr, key)); } \
static inline ctype php_array_fetchl_##ztype(zval *zarr, const char *key, int key_len) \
	{ return php_array_zval_to_##ztype(php_array_fetchl(zarr, key, key_len)); } \
static inline ctype php_array_fetchl_safe_##ztype(zval *zarr, const char *key, int key_len) \
	{ return php_array_zval_to_##ztype(php_array_fetchl_safe(zarr, key, key_len)); } \
static inline ctype php_array_fetchn_##ztype(zval *zarr, PAA_ULONG idx) \
	{ return php_array_zval_to_##ztype(php_array_fetchn(zarr, idx)); } \
static inline ctype php_array_fetchz_##ztype(zval *zarr, zval *key) \
	{ return php_array_zval_to_##ztype(php_array_fetchz(zarr, key)); }

/* Fetch zend_bool
 *
 * zend_bool php_array_fetch_bool(zval *zarr, const char *key)
 * zend_bool php_array_fetchl_bool(zval *zarr, const char *key, int key_len)
 * zend_bool php_array_fetchl_safe_bool(zval *zarr, const char *key, int key_len)
 * zend_bool php_array_fetchn_bool(zval *zarr, unsigned long idx)
 * zend_bool php_array_fetchc_bool(zval *zarr, const char *litstr)
 * zend_bool php_array_fetchz_bool(zval *zarr, zval *key)
 */
static inline
zend_bool php_array_zval_to_bool(zval *z) {
	return z && zend_is_true(z);
}
PHP_ARRAY_FETCH_TYPE_MAP(zend_bool, bool)
#define php_array_fetchc_bool(zarr, litstr) \
	php_array_zval_to_bool(php_array_fetchc(zarr, litstr))

/* Fetch long
 *
 * long php_array_fetch_long(zval *zarr, const char *key)
 * long php_array_fetchl_long(zval *zarr, const char *key, int key_len)
 * long php_array_fetchl_safe_long(zval *zarr, const char *key, int key_len)
 * long php_array_fetchn_long(zval *zarr, unsigned long idx)
 * long php_array_fetchc_long(zval *zarr, const char *litstr)
 * long php_array_fetchz_long(zval *zarr, zval *key)
 */
static inline
PAA_LONG php_array_zval_to_long(zval *z) {
	if (!z) { return 0; }
	switch(Z_TYPE_P(z)) {
		case IS_NULL: return 0;
#ifdef ZEND_ENGINE_3
		case IS_FALSE: return 0;
		case IS_TRUE: return 1;
#else
		case IS_BOOL: return Z_BVAL_P(z);
#endif
		case IS_LONG: return Z_LVAL_P(z);
		default:
		{
			zval c = *z;
			zval_copy_ctor(&c);
			convert_to_long(&c);
			return Z_LVAL(c);
		}
	}
}
PHP_ARRAY_FETCH_TYPE_MAP(PAA_LONG, long)
#define php_array_fetchc_long(zarr, litstr) \
	php_array_zval_to_long(php_array_fetchc(zarr, litstr))

/* Fetch double
 *
 * double php_array_fetch_double(zval *zarr, const char *key)
 * double php_array_fetchl_double(zval *zarr, const char *key, int key_len)
 * double php_array_fetchl_safe_double(zval *zarr, const char *key, int key_len)
 * double php_array_fetchn_double(zval *zarr, unsigned long idx)
 * double php_array_fetchc_double(zval *zarr, const char *litstr)
 * double php_array_fetchz_double(zval *zarr, zval *key)
 */
static inline
double php_array_zval_to_double(zval *z) {
	if (!z) { return 0.0; }
	switch (Z_TYPE_P(z)) {
		case IS_NULL: return 0.0;
#ifdef ZEND_ENGINE_3
		case IS_FALSE: return 0.0;
		case IS_TRUE: return 1.0;
#else
		case IS_BOOL: return (double)Z_BVAL_P(z);
#endif
		case IS_LONG: return (double)Z_LVAL_P(z);
		case IS_DOUBLE: return Z_DVAL_P(z);
		default:
		{
			zval c = *z;
			zval_copy_ctor(&c);
			convert_to_double(&c);
			return Z_DVAL(c);
		}
	}
}
PHP_ARRAY_FETCH_TYPE_MAP(double, double)
#define php_array_fetchc_double(zarr, litstr) \
	php_array_zval_to_double(php_array_fetchc(zarr, litstr))

/* Fetch string
 *
 * If the pfree is set to 1 on exit, then the return value is owned by the caller
 * and must be efree()'d once it is no longer in use.
 *
 * plen is populated with the binary safe length of the string returned.
 *
 * char *php_array_fetch_string(zval *zarr, const char *key, int *plen, zend_bool *pfree)
 * char *php_array_fetchl_string(zval *zarr, const char *key, int key_len, int *plen, zend_bool *pfree)
 * char *php_array_fetchl_safe_string(zval *zarr, const char *key, int key_len, int *plen, zend_bool *pfree)
 * char *php_array_fetchn_string(zval *zarr, unsigned long idx, int *plen, zend_bool *pfree)
 * char *php_array_fetchc_string(zval *zarr, const char *litstr, int *plen, zend_bool *pfree)
 * char *php_array_fetchz_string(zval *zarr, zval *key, int *plen, zend_bool *pfree)
 */
static inline
char *php_array_zval_to_string(zval *z, int *plen, zend_bool *pfree) {
	*plen = 0;
	*pfree = 0;
	if (!z) { return NULL; }
	switch (Z_TYPE_P(z)) {
		case IS_NULL:
			return (char *)"";
		case IS_STRING:
			*plen = Z_STRLEN_P(z);
			return Z_STRVAL_P(z);
		default:
		{
			zval c = *z;
			zval_copy_ctor(&c);
			convert_to_string(&c);
#ifdef ZEND_ENGINE_3
			*pfree = ! IS_INTERNED(Z_STR(c));
#else
			*pfree = ! IS_INTERNED(Z_STRVAL(c));
#endif
			*plen = Z_STRLEN(c);
			return Z_STRVAL(c);
		}
	}
}
#define php_array_fetch_string(zarr, key, plen, pfree) \
	php_array_zval_to_string(php_array_fetch(zarr, key), plen, pfree)
#define php_array_fetchl_string(zarr, key, key_len, plen, pfree) \
	php_array_zval_to_string(php_array_fetchl(zarr, key, key_len), plen, pfree)
#define php_array_fetchl_safe_string(zarr, key, key_len, plen, pfree) \
	php_array_zval_to_string(php_array_fetchl_safe(zarr, key, key_len), plen, pfree)
#define php_array_fetchn_string(zarr, idx, plen, pfree) \
	php_array_zval_to_string(php_array_fetchn(zarr, idx), plen, pfree)
#define php_array_fetchc_string(zarr, litstr, plen, pfree) \
	php_array_zval_to_string(php_array_fetchc(zarr, litstr), plen, pfree)
#define php_array_fetchz_string(zarr, key, plen, pfree) \
	php_array_zval_to_string(php_array_fetchz(zarr, key), plen, pfree)


/* Fetch array
 *
 * No implicit conversion is performed.
 *
 * If the value is an array, then that zval is returned,
 * otherwise NULL is returned.
 *
 * zval *php_array_fetch_array(zval *zarr, const char *key)
 * zval *php_array_fetchl_array(zval *zarr, const char *key, int key_len)
 * zval *php_array_fetchl_safe_array(zval *zarr, const char *key, int key_len)
 * zval *php_array_fetchn_array(zval *zarr, unsigned long idx)
 * zval *php_array_fetchc_array(zval *zarr, const char *litstr)
 * zval *php_array_fetchz_array(zval *zarr, zval *key)
 */
static inline zval *php_array_zval_to_array(zval *zarr) {
	return (zarr && (Z_TYPE_P(zarr) == IS_ARRAY)) ? zarr : NULL;
}
PHP_ARRAY_FETCH_TYPE_MAP(zval*, array)
#define php_array_fetchc_array(zarr, litstr) \
	php_array_zval_to_array(php_array_fetchc(zarr, litstr))

/* count($arr) - Count number of elements in the array
 *
 * int php_array_count(zval *arr)
 */
#define php_array_count(zarr) zend_hash_num_elements(Z_ARRVAL_P(zarr))

/* Fetch resource
 *
 * No implicit conversion is performed.
 *
 * If the value is a resource of the named type,
 * then the pointer for it is returned,
 * otherwise NULL is returned.
 *
 * To test for multiple resource types (e.g. 'stream' and 'persistent stream')
 * Fetch a generic zval* and use Zend's ZEND_FETCH_RESOURCE() macro.
 *
 * zval *php_array_fetch_resource(zval *zarr, const char *key, int le)
 * zval *php_array_fetchl_resource(zval *zarr, const char *key, int key_len, int le)
 * zval *php_array_fetchl_safe_resource(zval *zarr, const char *key, int key_len, int le)
 * zval *php_array_fetchn_resource(zval *zarr, unsigned long idx, int le)
 * zval *php_array_fetchc_resource(zval *zarr, const char *litstr, int le)
 * zval *php_array_fetchz_resource(zval *zarr, zval *key, int le)
 */
static inline
void *php_array_zval_to_resource(zval *z, int le TSRMLS_DC) {
#ifdef ZEND_ENGINE_3
	return zend_fetch_resource_ex(z, NULL, le);
#else
	void *ret;
	int rtype;
 	if (!z || Z_TYPE_P(z) != IS_RESOURCE) { return NULL; }
	ret = zend_list_find(Z_RESVAL_P(z), &rtype);
	if (!ret || (rtype != le)) {
		return NULL;
	}
	return ret;
#endif
}
#define php_array_fetch_resource(zarr, key, le) \
	php_array_zval_to_resource(php_array_fetch(zarr, key), le TSRMLS_CC)
#define php_array_fetchl_resource(zarr, key, key_len, le) \
	php_array_zval_to_resource(php_array_fetchl(zarr, key, key_len), le TSRMLS_CC)
#define php_array_fetchl_safe_resource(zarr, key, key_len, le) \
	php_array_zval_to_resource(php_array_fetchl_safe(zarr, key, key_len), le TSRMLS_CC)
#define php_array_fetchn_resource(zarr, idx, le) \
	php_array_zval_to_resource(php_array_fetchn(zarr, idx), le TSRMLS_CC)
#define php_array_fetchc_resource(zarr, litstr, le) \
	php_array_zval_to_resource(php_array_fetchc(zarr, litstr), le TSRMLS_CC)
#define php_array_fetchz_resource(zarr, key, le) \
        php_array_zval_to_resource(php_array_fetchz(zarr, key), le TSRMLS_CC)

/* Fetch Object
 *
 * Fetch an object of a specific or non-specific type (pass ce == NULL)
 *
 * No implicit conversion is performed
 *
 * zval *php_array_fetch_object(zval *zarr, const char *key, zend_class_entry *ce)
 * zval *php_array_fetchl_object(zval *zarr, const char *key, int key_len, zend_class_entry *ce)
 * zval *php_array_fetchl_safe_object(zval *zarr, const char *key, int key_len, zend_class_entry *ce)
 * zval *php_array_fetchn_object(zval *zarr, unsigned long idx, zend_class_entry *ce)
 * zval *php_array_fetchc_object(zval *zarr, const char *litstr, zend_class_entry *ce)
 * zval *php_array_fetchz_object(zval *zarr, zval *key, zend_class_entry *ce)
 */
static inline
zval *php_array_zval_to_object(zval *z, zend_class_entry *ce TSRMLS_DC) {
	return (z && (Z_TYPE_P(z) == IS_OBJECT) &&
	        ((!ce) || instanceof_function(Z_OBJCE_P(z), ce TSRMLS_CC))) ? z : NULL;
}
#define php_array_fetch_object(zarr, key, ce) \
	php_array_zval_to_object(php_array_fetch(zarr, key), ce TSRMLS_CC)
#define php_array_fetchl_object(zarr, key, len, ce) \
	php_array_zval_to_object(php_array_fetchl(zarr, key, len), ce TSRMLS_CC)
#define php_array_fetchl_safe_object(zarr, key, len, ce) \
	php_array_zval_to_object(php_array_fetchl_safe(zarr, key, len), ce TSRMLS_CC)
#define php_array_fetchn_object(zarr, idx, ce) \
	php_array_zval_to_object(php_array_fetchn(zarr, idx), ce TSRMLS_CC)
#define php_array_fetchc_object(zarr, litstr, ce) \
	php_array_zval_to_object(php_array_fetchc(zarr, litstr), ce TSRMLS_CC)
#define php_array_fetchz_object(zarr, key, ce) \
        php_array_zval_to_object(php_array_fetchz(zarr, key), ce TSRMLS_CC)

/* unset($zarr[$key]) - Erase a key from an array
 *
 * void php_array_unset(zval *zarr, const char *key)
 * void php_array_unsetl(zval *zarr, const char *key, int key_len)
 * void php_array_unsetl_safe(zval *zarr, const char *key, int key_len)
 * void php_array_unsetn(zval *zarr, long idx)
 * void php_array_unsetc(zval *zarr, const char *litstr)
 * void php_array_unsetz(zval *zarr, zval *key)
 */
static inline
void php_array_unset(zval *zarr, const char *key) {
	PAA_SYM_DEL(Z_ARRVAL_P(zarr), key, PAA_LENGTH_ADJ(strlen(key)));
}
#define php_array_unsetl(zarr, key, len) \
	PAA_SYM_DEL(Z_ARRVAL_P(zarr), key, PAA_LENGTH_ADJ(len))
static inline
void php_array_unsetl_safe(zval *zarr, const char *key, int key_len) {
	char *k = estrndup(key, key_len);
	PAA_SYM_DEL(Z_ARRVAL_P(zarr), k, PAA_LENGTH_ADJ(key_len));
	efree(k);
}
#define php_array_unsetn(zarr, idx) \
	zend_symtable_index_del(Z_ARRVAL_P(zarr), idx)
#define php_array_unsetc(zarr, litstr) \
	PAA_SYM_DEL(Z_ARRVAL_P(zarr), litstr, PAA_LENGTH_ADJ(sizeof(litstr) - 1))
static inline void php_array_unsetz(zval *zarr, zval *key) {
	switch (Z_TYPE_P(key)) {
		case IS_NULL:
			zend_hash_index_del(Z_ARRVAL_P(zarr), 0);
			return;
#ifdef ZEND_ENGINE_3
		case IS_FALSE:
			zend_hash_index_del(Z_ARRVAL_P(zarr), 0);
			return;
		case IS_TRUE:
			zend_hash_index_del(Z_ARRVAL_P(zarr), 1);
			return;
#else
		case IS_BOOL: /* fallthrough */
#endif
		case IS_LONG:
			zend_hash_index_del(Z_ARRVAL_P(zarr), Z_LVAL_P(key));
			return;
		case IS_DOUBLE:
			zend_hash_index_del(Z_ARRVAL_P(zarr), (long)Z_DVAL_P(key));
			break;
		case IS_STRING:
			php_array_unsetl(zarr, Z_STRVAL_P(key), Z_STRLEN_P(key));
			break;
	}
}

#endif /* PHP_ARRAY_API_H */

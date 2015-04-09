#ifndef PHP_FE_END
# define PHP_FE_END { NULL, NULL, NULL }
#endif

#ifndef HASH_KEY_NON_EXISTENT
# define HASH_KEY_NON_EXISTENT HASH_KEY_NON_EXISTANT
#endif

#if PHP_VERSION_ID < 50400
# define object_properties_init(_std, _class_type) \
		zend_hash_copy(*_std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
#endif

#if PHP_VERSION_ID >= 50500
#define ITERATOR_GET_CURRENT_KEY(it, z) \
		(it).funcs->get_current_key(&(it), z TSRMLS_CC);
#else
#define ITERATOR_GET_CURRENT_KEY(it, z) \
{ \
	char *str_key; \
	uint str_key_len; \
	ulong int_key; \
	switch ((it).funcs->get_current_key(&(it), &str_key, &str_key_len, &int_key TSRMLS_CC)) { \
		case HASH_KEY_IS_LONG: \
			ZVAL_LONG(z, int_key); \
			break; \
		case HASH_KEY_IS_STRING: \
			ZVAL_STRINGL(z, str_key, str_key_len-1, 0); \
			break; \
		default: \
			RETURN_NULL(); \
	} \
}
#endif

/* There was a very bad bug in 5.6.0 through 5.6.6 downgrading
 * METHOD_SSLv23 to SSLv2 and SSLv3 exclusively */
#if PHP_VERSION_ID >= 50600 && PHP_VERSION_ID < 50607
# define PHONGO_CRYPTO_METHOD STREAM_CRYPTO_METHOD_ANY_CLIENT
#else
# define PHONGO_CRYPTO_METHOD STREAM_CRYPTO_METHOD_SSLv23_CLIENT
#endif

#if defined(__GNUC__)
#  define ARG_UNUSED  __attribute__ ((unused))
#else
#  define ARG_UNUSED
#endif

#if SIZEOF_LONG == 4
# define ADD_INDEX_INT64(zval, index, value) \
	if (value > LONG_MAX || value < LONG_MIN) { \
		char *tmp; \
		int tmp_len; \
		mongoc_log(MONGOC_LOG_LEVEL_WARNING, MONGOC_LOG_DOMAIN, "Integer overflow detected on your platform: %lld", value); \
		tmp_len = spprintf(&tmp, 0, "%lld", value); \
		add_index_stringl(zval, index, tmp, tmp_len, 0); \
	} else { \
		add_index_long(zval, index, val); \
	}
# define ADD_ASSOC_INT64(zval, key, value) \
	if (value > LONG_MAX || value < LONG_MIN) { \
		char *tmp; \
		int tmp_len; \
		mongoc_log(MONGOC_LOG_LEVEL_WARNING, MONGOC_LOG_DOMAIN, "Integer overflow detected on your platform: %lld", value); \
		tmp_len = spprintf(&tmp, 0, "%lld", value); \
		add_assoc_stringl(zval, key, tmp, tmp_len, 0); \
	} else { \
		add_assoc_long(zval, key, value); \
	}
#else
# define ADD_INDEX_INT64(zval, index, value) add_index_long(zval, index, value)
# define ADD_ASSOC_INT64(zval, key, value) add_assoc_long(zval, key, value);
#endif

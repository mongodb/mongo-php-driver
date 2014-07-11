#ifndef PHP_FE_END
# define PHP_FE_END { NULL, NULL, NULL }
#endif

#ifndef HASH_KEY_NON_EXISTENT
# define HASH_KEY_NON_EXISTENT HASH_KEY_NON_EXISTANT
#endif

#if PHP_VERSION_ID < 50400
# define object_properties_init(_std, _class_type) \
		zend_hash_copy(_std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
#endif

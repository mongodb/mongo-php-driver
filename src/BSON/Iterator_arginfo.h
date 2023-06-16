/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3c935268e2187085468340004c8dc67052d5c19b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Iterator___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Iterator_current, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Iterator_current, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_Iterator_key, 0, 0, MAY_BE_STRING|MAY_BE_LONG)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
#define arginfo_class_MongoDB_BSON_Iterator_key arginfo_class_MongoDB_BSON_Iterator_current
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Iterator_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Iterator_rewind arginfo_class_MongoDB_BSON_Iterator_next

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Iterator_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Iterator___wakeup arginfo_class_MongoDB_BSON_Iterator_next


static ZEND_METHOD(MongoDB_BSON_Iterator, __construct);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Iterator, current);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Iterator, current);
#endif
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Iterator, key);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Iterator, key);
#endif
static ZEND_METHOD(MongoDB_BSON_Iterator, next);
static ZEND_METHOD(MongoDB_BSON_Iterator, rewind);
static ZEND_METHOD(MongoDB_BSON_Iterator, valid);
static ZEND_METHOD(MongoDB_BSON_Iterator, __wakeup);


static const zend_function_entry class_MongoDB_BSON_Iterator_methods[] = {
	ZEND_ME(MongoDB_BSON_Iterator, __construct, arginfo_class_MongoDB_BSON_Iterator___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Iterator, current, arginfo_class_MongoDB_BSON_Iterator_current, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Iterator, current, arginfo_class_MongoDB_BSON_Iterator_current, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Iterator, key, arginfo_class_MongoDB_BSON_Iterator_key, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Iterator, key, arginfo_class_MongoDB_BSON_Iterator_key, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_Iterator, next, arginfo_class_MongoDB_BSON_Iterator_next, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Iterator, rewind, arginfo_class_MongoDB_BSON_Iterator_rewind, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Iterator, valid, arginfo_class_MongoDB_BSON_Iterator_valid, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Iterator, __wakeup, arginfo_class_MongoDB_BSON_Iterator___wakeup, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Iterator(zend_class_entry *class_entry_Iterator)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Iterator", class_MongoDB_BSON_Iterator_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_Iterator);

	return class_entry;
}

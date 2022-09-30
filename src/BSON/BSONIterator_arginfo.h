/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b258becdef5c36e38f0cc57f472c95d03307660f */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_BSONIterator___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONIterator_current, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_BSONIterator_current, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_BSONIterator_key, 0, 0, MAY_BE_STRING|MAY_BE_LONG)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
#define arginfo_class_MongoDB_BSON_BSONIterator_key arginfo_class_MongoDB_BSON_BSONIterator_current
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONIterator_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_BSONIterator_rewind arginfo_class_MongoDB_BSON_BSONIterator_next

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONIterator_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_BSONIterator___wakeup arginfo_class_MongoDB_BSON_BSONIterator_next


static ZEND_METHOD(MongoDB_BSON_BSONIterator, __construct);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_BSONIterator, current);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_BSONIterator, current);
#endif
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_BSONIterator, key);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_BSONIterator, key);
#endif
static ZEND_METHOD(MongoDB_BSON_BSONIterator, next);
static ZEND_METHOD(MongoDB_BSON_BSONIterator, rewind);
static ZEND_METHOD(MongoDB_BSON_BSONIterator, valid);
static ZEND_METHOD(MongoDB_BSON_BSONIterator, __wakeup);


static const zend_function_entry class_MongoDB_BSON_BSONIterator_methods[] = {
	ZEND_ME(MongoDB_BSON_BSONIterator, __construct, arginfo_class_MongoDB_BSON_BSONIterator___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_BSONIterator, current, arginfo_class_MongoDB_BSON_BSONIterator_current, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_BSONIterator, current, arginfo_class_MongoDB_BSON_BSONIterator_current, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_BSONIterator, key, arginfo_class_MongoDB_BSON_BSONIterator_key, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_BSONIterator, key, arginfo_class_MongoDB_BSON_BSONIterator_key, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_BSONIterator, next, arginfo_class_MongoDB_BSON_BSONIterator_next, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONIterator, rewind, arginfo_class_MongoDB_BSON_BSONIterator_rewind, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONIterator, valid, arginfo_class_MongoDB_BSON_BSONIterator_valid, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONIterator, __wakeup, arginfo_class_MongoDB_BSON_BSONIterator___wakeup, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_BSONIterator(zend_class_entry *class_entry_Iterator)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "BSONIterator", class_MongoDB_BSON_BSONIterator_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_Iterator);

	return class_entry;
}

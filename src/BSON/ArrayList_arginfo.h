/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d8beb2fc73058457fd8e835500b13249583d413a */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList_fromPHP, 0, 1, MongoDB\\BSON\\ArrayList, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList_get, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList_getIterator, 0, 0, MongoDB\\BSON\\Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList_has, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList_toPHP, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_ArrayList___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_ArrayList, __construct);
static ZEND_METHOD(MongoDB_BSON_ArrayList, fromPHP);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_ArrayList, get);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_ArrayList, get);
#endif
static ZEND_METHOD(MongoDB_BSON_ArrayList, getIterator);
static ZEND_METHOD(MongoDB_BSON_ArrayList, has);
static ZEND_METHOD(MongoDB_BSON_ArrayList, toPHP);
static ZEND_METHOD(MongoDB_BSON_ArrayList, __toString);


static const zend_function_entry class_MongoDB_BSON_ArrayList_methods[] = {
	ZEND_ME(MongoDB_BSON_ArrayList, __construct, arginfo_class_MongoDB_BSON_ArrayList___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(MongoDB_BSON_ArrayList, fromPHP, arginfo_class_MongoDB_BSON_ArrayList_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_ArrayList, get, arginfo_class_MongoDB_BSON_ArrayList_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_ArrayList, get, arginfo_class_MongoDB_BSON_ArrayList_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_ArrayList, getIterator, arginfo_class_MongoDB_BSON_ArrayList_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_ArrayList, has, arginfo_class_MongoDB_BSON_ArrayList_has, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_ArrayList, toPHP, arginfo_class_MongoDB_BSON_ArrayList_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_ArrayList, __toString, arginfo_class_MongoDB_BSON_ArrayList___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_ArrayList(zend_class_entry *class_entry_IteratorAggregate)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "ArrayList", class_MongoDB_BSON_ArrayList_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_IteratorAggregate);

	return class_entry;
}

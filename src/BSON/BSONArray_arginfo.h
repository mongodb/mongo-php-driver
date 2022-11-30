/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: d1b66ddfb49985fe0f81874d868bf3c807d64713 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray_fromPHP, 0, 1, MongoDB\\BSON\\BSONArray, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray_get, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray_getIterator, 0, 0, MongoDB\\BSON\\BSONIterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray_has, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray_toPHP, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONArray___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_BSONArray, __construct);
static ZEND_METHOD(MongoDB_BSON_BSONArray, fromPHP);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_BSONArray, get);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_BSONArray, get);
#endif
static ZEND_METHOD(MongoDB_BSON_BSONArray, getIterator);
static ZEND_METHOD(MongoDB_BSON_BSONArray, has);
static ZEND_METHOD(MongoDB_BSON_BSONArray, toPHP);
static ZEND_METHOD(MongoDB_BSON_BSONArray, __toString);


static const zend_function_entry class_MongoDB_BSON_BSONArray_methods[] = {
	ZEND_ME(MongoDB_BSON_BSONArray, __construct, arginfo_class_MongoDB_BSON_BSONArray___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(MongoDB_BSON_BSONArray, fromPHP, arginfo_class_MongoDB_BSON_BSONArray_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_BSONArray, get, arginfo_class_MongoDB_BSON_BSONArray_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_BSONArray, get, arginfo_class_MongoDB_BSON_BSONArray_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_BSONArray, getIterator, arginfo_class_MongoDB_BSON_BSONArray_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONArray, has, arginfo_class_MongoDB_BSON_BSONArray_has, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONArray, toPHP, arginfo_class_MongoDB_BSON_BSONArray_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONArray, __toString, arginfo_class_MongoDB_BSON_BSONArray___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_BSONArray(zend_class_entry *class_entry_IteratorAggregate)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "BSONArray", class_MongoDB_BSON_BSONArray_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_IteratorAggregate);

	return class_entry;
}

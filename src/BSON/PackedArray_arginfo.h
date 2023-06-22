/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e9b57c4051440170531560cc355e0af1b43f3424 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_fromPHP, 0, 1, MongoDB\\BSON\\PackedArray, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_get, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_getIterator, 0, 0, MongoDB\\BSON\\Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_has, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_PackedArray_toPHP, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_toPHP, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___set_state, 0, 1, MongoDB\\BSON\\PackedArray, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_PackedArray_serialize arginfo_class_MongoDB_BSON_PackedArray___toString

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_PackedArray___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_PackedArray, __construct);
static ZEND_METHOD(MongoDB_BSON_PackedArray, fromPHP);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_PackedArray, get);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_PackedArray, get);
#endif
static ZEND_METHOD(MongoDB_BSON_PackedArray, getIterator);
static ZEND_METHOD(MongoDB_BSON_PackedArray, has);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_PackedArray, toPHP);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_PackedArray, toPHP);
#endif
static ZEND_METHOD(MongoDB_BSON_PackedArray, __toString);
static ZEND_METHOD(MongoDB_BSON_PackedArray, __set_state);
static ZEND_METHOD(MongoDB_BSON_PackedArray, serialize);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_PackedArray, unserialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_PackedArray, unserialize);
#endif
static ZEND_METHOD(MongoDB_BSON_PackedArray, __unserialize);
static ZEND_METHOD(MongoDB_BSON_PackedArray, __serialize);


static const zend_function_entry class_MongoDB_BSON_PackedArray_methods[] = {
	ZEND_ME(MongoDB_BSON_PackedArray, __construct, arginfo_class_MongoDB_BSON_PackedArray___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(MongoDB_BSON_PackedArray, fromPHP, arginfo_class_MongoDB_BSON_PackedArray_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_PackedArray, get, arginfo_class_MongoDB_BSON_PackedArray_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_PackedArray, get, arginfo_class_MongoDB_BSON_PackedArray_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_PackedArray, getIterator, arginfo_class_MongoDB_BSON_PackedArray_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, has, arginfo_class_MongoDB_BSON_PackedArray_has, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_PackedArray, toPHP, arginfo_class_MongoDB_BSON_PackedArray_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_PackedArray, toPHP, arginfo_class_MongoDB_BSON_PackedArray_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_PackedArray, __toString, arginfo_class_MongoDB_BSON_PackedArray___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, __set_state, arginfo_class_MongoDB_BSON_PackedArray___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, serialize, arginfo_class_MongoDB_BSON_PackedArray_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_PackedArray, unserialize, arginfo_class_MongoDB_BSON_PackedArray_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_PackedArray, unserialize, arginfo_class_MongoDB_BSON_PackedArray_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_PackedArray, __unserialize, arginfo_class_MongoDB_BSON_PackedArray___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_PackedArray, __serialize, arginfo_class_MongoDB_BSON_PackedArray___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_PackedArray(zend_class_entry *class_entry_IteratorAggregate, zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "PackedArray", class_MongoDB_BSON_PackedArray_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 2, class_entry_IteratorAggregate, class_entry_Serializable);

	return class_entry;
}

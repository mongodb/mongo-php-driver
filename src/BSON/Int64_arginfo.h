/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 86352e04686201023d5cad5ee577c8a3ce324628 */

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Int64___construct, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_STRING, NULL)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Int64___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Int64___set_state, 0, 1, MongoDB\\BSON\\Int64, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Int64_serialize arginfo_class_MongoDB_BSON_Int64___toString

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64_unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Int64_jsonSerialize, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Int64_jsonSerialize, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif


#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Int64, __construct);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Int64, __construct);
#endif
static ZEND_METHOD(MongoDB_BSON_Int64, __toString);
static ZEND_METHOD(MongoDB_BSON_Int64, __set_state);
static ZEND_METHOD(MongoDB_BSON_Int64, serialize);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Int64, unserialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Int64, unserialize);
#endif
static ZEND_METHOD(MongoDB_BSON_Int64, __unserialize);
static ZEND_METHOD(MongoDB_BSON_Int64, __serialize);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Int64, jsonSerialize);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Int64, jsonSerialize);
#endif


static const zend_function_entry class_MongoDB_BSON_Int64_methods[] = {
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Int64, __construct, arginfo_class_MongoDB_BSON_Int64___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Int64, __construct, arginfo_class_MongoDB_BSON_Int64___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_Int64, __toString, arginfo_class_MongoDB_BSON_Int64___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, __set_state, arginfo_class_MongoDB_BSON_Int64___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, serialize, arginfo_class_MongoDB_BSON_Int64_serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Int64, unserialize, arginfo_class_MongoDB_BSON_Int64_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Int64, unserialize, arginfo_class_MongoDB_BSON_Int64_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_Int64, __unserialize, arginfo_class_MongoDB_BSON_Int64___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Int64, __serialize, arginfo_class_MongoDB_BSON_Int64___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Int64, jsonSerialize, arginfo_class_MongoDB_BSON_Int64_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Int64, jsonSerialize, arginfo_class_MongoDB_BSON_Int64_jsonSerialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Int64(zend_class_entry *class_entry_JsonSerializable, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Serializable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Int64", class_MongoDB_BSON_Int64_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 3, class_entry_JsonSerializable, class_entry_MongoDB_BSON_Type, class_entry_Serializable);

	return class_entry;
}

/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f88c15678c2bc90b6f3618c6081426b0fb3add06 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_fromJSON, 0, 1, MongoDB\\BSON\\BSONDocument, 0)
	ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_fromPHP, 0, 1, MongoDB\\BSON\\BSONDocument, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_fromPHP, 0, 1, MongoDB\\BSON\\BSONDocument, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
#endif

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_get, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_getIterator, 0, 0, MongoDB\\BSON\\BSONIterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_has, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_toPHP, 0, 0, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_toPHP, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_BSONDocument_toCanonicalExtendedJSON, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_BSONDocument_toRelaxedExtendedJSON arginfo_class_MongoDB_BSON_BSONDocument_toCanonicalExtendedJSON

#define arginfo_class_MongoDB_BSON_BSONDocument___toString arginfo_class_MongoDB_BSON_BSONDocument_toCanonicalExtendedJSON


static ZEND_METHOD(MongoDB_BSON_BSONDocument, __construct);
static ZEND_METHOD(MongoDB_BSON_BSONDocument, fromJSON);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_BSONDocument, fromPHP);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_BSONDocument, fromPHP);
#endif
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_BSONDocument, get);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_BSONDocument, get);
#endif
static ZEND_METHOD(MongoDB_BSON_BSONDocument, getIterator);
static ZEND_METHOD(MongoDB_BSON_BSONDocument, has);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_BSONDocument, toPHP);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_BSONDocument, toPHP);
#endif
static ZEND_METHOD(MongoDB_BSON_BSONDocument, toCanonicalExtendedJSON);
static ZEND_METHOD(MongoDB_BSON_BSONDocument, toRelaxedExtendedJSON);
static ZEND_METHOD(MongoDB_BSON_BSONDocument, __toString);


static const zend_function_entry class_MongoDB_BSON_BSONDocument_methods[] = {
	ZEND_ME(MongoDB_BSON_BSONDocument, __construct, arginfo_class_MongoDB_BSON_BSONDocument___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(MongoDB_BSON_BSONDocument, fromJSON, arginfo_class_MongoDB_BSON_BSONDocument_fromJSON, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_BSONDocument, fromPHP, arginfo_class_MongoDB_BSON_BSONDocument_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_BSONDocument, fromPHP, arginfo_class_MongoDB_BSON_BSONDocument_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_BSONDocument, get, arginfo_class_MongoDB_BSON_BSONDocument_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_BSONDocument, get, arginfo_class_MongoDB_BSON_BSONDocument_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_BSONDocument, getIterator, arginfo_class_MongoDB_BSON_BSONDocument_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONDocument, has, arginfo_class_MongoDB_BSON_BSONDocument_has, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_BSONDocument, toPHP, arginfo_class_MongoDB_BSON_BSONDocument_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_BSONDocument, toPHP, arginfo_class_MongoDB_BSON_BSONDocument_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_BSONDocument, toCanonicalExtendedJSON, arginfo_class_MongoDB_BSON_BSONDocument_toCanonicalExtendedJSON, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONDocument, toRelaxedExtendedJSON, arginfo_class_MongoDB_BSON_BSONDocument_toRelaxedExtendedJSON, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_BSONDocument, __toString, arginfo_class_MongoDB_BSON_BSONDocument___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_BSONDocument(zend_class_entry *class_entry_IteratorAggregate)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "BSONDocument", class_MongoDB_BSON_BSONDocument_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 1, class_entry_IteratorAggregate);

	return class_entry;
}

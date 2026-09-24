/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 13d114590a7c0bc86da4dbb1383d17c296edcb40 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Document___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Document_fromBSON, 0, 1, MongoDB\\BSON\\Document, 0)
	ZEND_ARG_TYPE_INFO(0, bson, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Document_fromJSON, 0, 1, MongoDB\\BSON\\Document, 0)
	ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Document_fromPHP, 0, 1, MongoDB\\BSON\\Document, 0)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document_get, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Document_getIterator, 0, 0, MongoDB\\BSON\\Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document_has, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_MongoDB_BSON_Document_toPHP, 0, 0, MAY_BE_ARRAY|MAY_BE_OBJECT)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, typeMap, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document_toCanonicalExtendedJSON, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Document_toRelaxedExtendedJSON arginfo_class_MongoDB_BSON_Document_toCanonicalExtendedJSON

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document_offsetExists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document_offsetGet, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document_offsetSet, 0, 2, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document_offsetUnset, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, offset, IS_MIXED, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Document___toString arginfo_class_MongoDB_BSON_Document_toCanonicalExtendedJSON

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Document___set_state, 0, 1, MongoDB\\BSON\\Document, 0)
	ZEND_ARG_TYPE_INFO(0, properties, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Document___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_BSON_Document, __construct);
static ZEND_METHOD(MongoDB_BSON_Document, fromBSON);
static ZEND_METHOD(MongoDB_BSON_Document, fromJSON);
static ZEND_METHOD(MongoDB_BSON_Document, fromPHP);
static ZEND_METHOD(MongoDB_BSON_Document, get);
static ZEND_METHOD(MongoDB_BSON_Document, getIterator);
static ZEND_METHOD(MongoDB_BSON_Document, has);
static ZEND_METHOD(MongoDB_BSON_Document, toPHP);
static ZEND_METHOD(MongoDB_BSON_Document, toCanonicalExtendedJSON);
static ZEND_METHOD(MongoDB_BSON_Document, toRelaxedExtendedJSON);
static ZEND_METHOD(MongoDB_BSON_Document, offsetExists);
static ZEND_METHOD(MongoDB_BSON_Document, offsetGet);
static ZEND_METHOD(MongoDB_BSON_Document, offsetSet);
static ZEND_METHOD(MongoDB_BSON_Document, offsetUnset);
static ZEND_METHOD(MongoDB_BSON_Document, __toString);
static ZEND_METHOD(MongoDB_BSON_Document, __set_state);
static ZEND_METHOD(MongoDB_BSON_Document, __unserialize);
static ZEND_METHOD(MongoDB_BSON_Document, __serialize);


static const zend_function_entry class_MongoDB_BSON_Document_methods[] = {
	ZEND_ME(MongoDB_BSON_Document, __construct, arginfo_class_MongoDB_BSON_Document___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(MongoDB_BSON_Document, fromBSON, arginfo_class_MongoDB_BSON_Document_fromBSON, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, fromJSON, arginfo_class_MongoDB_BSON_Document_fromJSON, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, fromPHP, arginfo_class_MongoDB_BSON_Document_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, get, arginfo_class_MongoDB_BSON_Document_get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, getIterator, arginfo_class_MongoDB_BSON_Document_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, has, arginfo_class_MongoDB_BSON_Document_has, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, toPHP, arginfo_class_MongoDB_BSON_Document_toPHP, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, toCanonicalExtendedJSON, arginfo_class_MongoDB_BSON_Document_toCanonicalExtendedJSON, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, toRelaxedExtendedJSON, arginfo_class_MongoDB_BSON_Document_toRelaxedExtendedJSON, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, offsetExists, arginfo_class_MongoDB_BSON_Document_offsetExists, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_Document, offsetGet, arginfo_class_MongoDB_BSON_Document_offsetGet, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_Document, offsetSet, arginfo_class_MongoDB_BSON_Document_offsetSet, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_Document, offsetUnset, arginfo_class_MongoDB_BSON_Document_offsetUnset, ZEND_ACC_PUBLIC)
	ZEND_ME(MongoDB_BSON_Document, __toString, arginfo_class_MongoDB_BSON_Document___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, __set_state, arginfo_class_MongoDB_BSON_Document___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, __unserialize, arginfo_class_MongoDB_BSON_Document___unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Document, __serialize, arginfo_class_MongoDB_BSON_Document___serialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Document(zend_class_entry *class_entry_IteratorAggregate, zend_class_entry *class_entry_ArrayAccess, zend_class_entry *class_entry_MongoDB_BSON_Type, zend_class_entry *class_entry_Stringable)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Document", class_MongoDB_BSON_Document_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;
	zend_class_implements(class_entry, 4, class_entry_IteratorAggregate, class_entry_ArrayAccess, class_entry_MongoDB_BSON_Type, class_entry_Stringable);

	return class_entry;
}

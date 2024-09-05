/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 29a1460703051cab24abd6b42228c90928e18049 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_addKeyAltName, 0, 2, IS_OBJECT, 1)
	ZEND_ARG_OBJ_INFO(0, keyId, MongoDB\\BSON\\Binary, 0)
	ZEND_ARG_TYPE_INFO(0, keyAltName, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_createDataKey, 0, 1, MongoDB\\BSON\\Binary, 0)
	ZEND_ARG_TYPE_INFO(0, kmsProvider, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_decrypt, 0, 1, IS_MIXED, 0)
	ZEND_ARG_OBJ_INFO(0, value, MongoDB\\BSON\\Binary, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_deleteKey, 0, 1, IS_OBJECT, 0)
	ZEND_ARG_OBJ_INFO(0, keyId, MongoDB\\BSON\\Binary, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_encrypt, 0, 1, MongoDB\\BSON\\Binary, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_encryptExpression, 0, 1, IS_OBJECT, 0)
	ZEND_ARG_TYPE_MASK(0, expr, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_getKey, 0, 1, IS_OBJECT, 1)
	ZEND_ARG_OBJ_INFO(0, keyId, MongoDB\\BSON\\Binary, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_getKeyByAltName, 0, 1, IS_OBJECT, 1)
	ZEND_ARG_TYPE_INFO(0, keyAltName, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_getKeys, 0, 0, MongoDB\\Driver\\Cursor, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_ClientEncryption_removeKeyAltName arginfo_class_MongoDB_Driver_ClientEncryption_addKeyAltName

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_ClientEncryption_rewrapManyDataKey, 0, 1, IS_OBJECT, 0)
	ZEND_ARG_TYPE_MASK(0, filter, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_ClientEncryption, __construct);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, addKeyAltName);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, createDataKey);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, decrypt);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, deleteKey);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, encrypt);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, encryptExpression);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, getKey);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, getKeyByAltName);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, getKeys);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, removeKeyAltName);
static ZEND_METHOD(MongoDB_Driver_ClientEncryption, rewrapManyDataKey);


static const zend_function_entry class_MongoDB_Driver_ClientEncryption_methods[] = {
	ZEND_ME(MongoDB_Driver_ClientEncryption, __construct, arginfo_class_MongoDB_Driver_ClientEncryption___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, addKeyAltName, arginfo_class_MongoDB_Driver_ClientEncryption_addKeyAltName, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, createDataKey, arginfo_class_MongoDB_Driver_ClientEncryption_createDataKey, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, decrypt, arginfo_class_MongoDB_Driver_ClientEncryption_decrypt, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, deleteKey, arginfo_class_MongoDB_Driver_ClientEncryption_deleteKey, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, encrypt, arginfo_class_MongoDB_Driver_ClientEncryption_encrypt, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, encryptExpression, arginfo_class_MongoDB_Driver_ClientEncryption_encryptExpression, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, getKey, arginfo_class_MongoDB_Driver_ClientEncryption_getKey, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, getKeyByAltName, arginfo_class_MongoDB_Driver_ClientEncryption_getKeyByAltName, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, getKeys, arginfo_class_MongoDB_Driver_ClientEncryption_getKeys, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, removeKeyAltName, arginfo_class_MongoDB_Driver_ClientEncryption_removeKeyAltName, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_ClientEncryption, rewrapManyDataKey, arginfo_class_MongoDB_Driver_ClientEncryption_rewrapManyDataKey, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_ClientEncryption(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ClientEncryption", class_MongoDB_Driver_ClientEncryption_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	zval const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_value;
	zend_string *const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_value_str = zend_string_init(MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC, strlen(MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC), 1);
	ZVAL_STR(&const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_value, const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_value_str);
	zend_string *const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_name = zend_string_init_interned("AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC", sizeof("AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_name, &const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC_name);

	zval const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_value;
	zend_string *const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_value_str = zend_string_init(MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM, strlen(MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM), 1);
	ZVAL_STR(&const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_value, const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_value_str);
	zend_string *const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_name = zend_string_init_interned("AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM", sizeof("AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_name, &const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM_name);

	zval const_ALGORITHM_INDEXED_value;
	zend_string *const_ALGORITHM_INDEXED_value_str = zend_string_init(MONGOC_ENCRYPT_ALGORITHM_INDEXED, strlen(MONGOC_ENCRYPT_ALGORITHM_INDEXED), 1);
	ZVAL_STR(&const_ALGORITHM_INDEXED_value, const_ALGORITHM_INDEXED_value_str);
	zend_string *const_ALGORITHM_INDEXED_name = zend_string_init_interned("ALGORITHM_INDEXED", sizeof("ALGORITHM_INDEXED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_ALGORITHM_INDEXED_name, &const_ALGORITHM_INDEXED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_ALGORITHM_INDEXED_name);

	zval const_ALGORITHM_UNINDEXED_value;
	zend_string *const_ALGORITHM_UNINDEXED_value_str = zend_string_init(MONGOC_ENCRYPT_ALGORITHM_UNINDEXED, strlen(MONGOC_ENCRYPT_ALGORITHM_UNINDEXED), 1);
	ZVAL_STR(&const_ALGORITHM_UNINDEXED_value, const_ALGORITHM_UNINDEXED_value_str);
	zend_string *const_ALGORITHM_UNINDEXED_name = zend_string_init_interned("ALGORITHM_UNINDEXED", sizeof("ALGORITHM_UNINDEXED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_ALGORITHM_UNINDEXED_name, &const_ALGORITHM_UNINDEXED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_ALGORITHM_UNINDEXED_name);

	zval const_ALGORITHM_RANGE_value;
	zend_string *const_ALGORITHM_RANGE_value_str = zend_string_init(MONGOC_ENCRYPT_ALGORITHM_RANGE, strlen(MONGOC_ENCRYPT_ALGORITHM_RANGE), 1);
	ZVAL_STR(&const_ALGORITHM_RANGE_value, const_ALGORITHM_RANGE_value_str);
	zend_string *const_ALGORITHM_RANGE_name = zend_string_init_interned("ALGORITHM_RANGE", sizeof("ALGORITHM_RANGE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_ALGORITHM_RANGE_name, &const_ALGORITHM_RANGE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_ALGORITHM_RANGE_name);

	zval const_ALGORITHM_RANGE_PREVIEW_value;
	zend_string *const_ALGORITHM_RANGE_PREVIEW_value_str = zend_string_init(MONGOC_ENCRYPT_ALGORITHM_RANGEPREVIEW, strlen(MONGOC_ENCRYPT_ALGORITHM_RANGEPREVIEW), 1);
	ZVAL_STR(&const_ALGORITHM_RANGE_PREVIEW_value, const_ALGORITHM_RANGE_PREVIEW_value_str);
	zend_string *const_ALGORITHM_RANGE_PREVIEW_name = zend_string_init_interned("ALGORITHM_RANGE_PREVIEW", sizeof("ALGORITHM_RANGE_PREVIEW") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_ALGORITHM_RANGE_PREVIEW_name, &const_ALGORITHM_RANGE_PREVIEW_value, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED, NULL);
	zend_string_release(const_ALGORITHM_RANGE_PREVIEW_name);

	zval const_QUERY_TYPE_EQUALITY_value;
	zend_string *const_QUERY_TYPE_EQUALITY_value_str = zend_string_init(MONGOC_ENCRYPT_QUERY_TYPE_EQUALITY, strlen(MONGOC_ENCRYPT_QUERY_TYPE_EQUALITY), 1);
	ZVAL_STR(&const_QUERY_TYPE_EQUALITY_value, const_QUERY_TYPE_EQUALITY_value_str);
	zend_string *const_QUERY_TYPE_EQUALITY_name = zend_string_init_interned("QUERY_TYPE_EQUALITY", sizeof("QUERY_TYPE_EQUALITY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_QUERY_TYPE_EQUALITY_name, &const_QUERY_TYPE_EQUALITY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_QUERY_TYPE_EQUALITY_name);

	zval const_QUERY_TYPE_RANGE_value;
	zend_string *const_QUERY_TYPE_RANGE_value_str = zend_string_init(MONGOC_ENCRYPT_QUERY_TYPE_RANGE, strlen(MONGOC_ENCRYPT_QUERY_TYPE_RANGE), 1);
	ZVAL_STR(&const_QUERY_TYPE_RANGE_value, const_QUERY_TYPE_RANGE_value_str);
	zend_string *const_QUERY_TYPE_RANGE_name = zend_string_init_interned("QUERY_TYPE_RANGE", sizeof("QUERY_TYPE_RANGE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_QUERY_TYPE_RANGE_name, &const_QUERY_TYPE_RANGE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_QUERY_TYPE_RANGE_name);

	zval const_QUERY_TYPE_RANGE_PREVIEW_value;
	zend_string *const_QUERY_TYPE_RANGE_PREVIEW_value_str = zend_string_init(MONGOC_ENCRYPT_QUERY_TYPE_RANGEPREVIEW, strlen(MONGOC_ENCRYPT_QUERY_TYPE_RANGEPREVIEW), 1);
	ZVAL_STR(&const_QUERY_TYPE_RANGE_PREVIEW_value, const_QUERY_TYPE_RANGE_PREVIEW_value_str);
	zend_string *const_QUERY_TYPE_RANGE_PREVIEW_name = zend_string_init_interned("QUERY_TYPE_RANGE_PREVIEW", sizeof("QUERY_TYPE_RANGE_PREVIEW") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_QUERY_TYPE_RANGE_PREVIEW_name, &const_QUERY_TYPE_RANGE_PREVIEW_value, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED, NULL);
	zend_string_release(const_QUERY_TYPE_RANGE_PREVIEW_name);

	return class_entry;
}

/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 995186f86af6cd309bb91f7165aac5d84b174284 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Value___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Value_getType, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Value_getValue, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_BSON_Value_getValue, 0, 0, 0)
ZEND_END_ARG_INFO()
#endif

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Value_fromPHP, 0, 1, MongoDB\\BSON\\Value, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_MIXED, 0)
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_BSON_Value_fromPHP, 0, 1, MongoDB\\BSON\\Value, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_BSON_Value_isArray, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_BSON_Value_isBinary arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isBool arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isCode arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isDBPointer arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isDecimal128 arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isDocument arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isDouble arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isInt arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isInt32 arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isInt64 arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isMaxKey arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isMinKey arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isNumber arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isNull arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isObjectId arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isRegex arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isSymbol arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isTimestamp arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isUndefined arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isUTCDateTime arginfo_class_MongoDB_BSON_Value_isArray

#define arginfo_class_MongoDB_BSON_Value_isUtf8 arginfo_class_MongoDB_BSON_Value_isArray


static ZEND_METHOD(MongoDB_BSON_Value, __construct);
static ZEND_METHOD(MongoDB_BSON_Value, getType);
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Value, getValue);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Value, getValue);
#endif
#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_BSON_Value, fromPHP);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_BSON_Value, fromPHP);
#endif
static ZEND_METHOD(MongoDB_BSON_Value, isArray);
static ZEND_METHOD(MongoDB_BSON_Value, isBinary);
static ZEND_METHOD(MongoDB_BSON_Value, isBool);
static ZEND_METHOD(MongoDB_BSON_Value, isCode);
static ZEND_METHOD(MongoDB_BSON_Value, isDBPointer);
static ZEND_METHOD(MongoDB_BSON_Value, isDecimal128);
static ZEND_METHOD(MongoDB_BSON_Value, isDocument);
static ZEND_METHOD(MongoDB_BSON_Value, isDouble);
static ZEND_METHOD(MongoDB_BSON_Value, isInt);
static ZEND_METHOD(MongoDB_BSON_Value, isInt32);
static ZEND_METHOD(MongoDB_BSON_Value, isInt64);
static ZEND_METHOD(MongoDB_BSON_Value, isMaxKey);
static ZEND_METHOD(MongoDB_BSON_Value, isMinKey);
static ZEND_METHOD(MongoDB_BSON_Value, isNumber);
static ZEND_METHOD(MongoDB_BSON_Value, isNull);
static ZEND_METHOD(MongoDB_BSON_Value, isObjectId);
static ZEND_METHOD(MongoDB_BSON_Value, isRegex);
static ZEND_METHOD(MongoDB_BSON_Value, isSymbol);
static ZEND_METHOD(MongoDB_BSON_Value, isTimestamp);
static ZEND_METHOD(MongoDB_BSON_Value, isUndefined);
static ZEND_METHOD(MongoDB_BSON_Value, isUTCDateTime);
static ZEND_METHOD(MongoDB_BSON_Value, isUtf8);


static const zend_function_entry class_MongoDB_BSON_Value_methods[] = {
	ZEND_ME(MongoDB_BSON_Value, __construct, arginfo_class_MongoDB_BSON_Value___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(MongoDB_BSON_Value, getType, arginfo_class_MongoDB_BSON_Value_getType, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Value, getValue, arginfo_class_MongoDB_BSON_Value_getValue, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Value, getValue, arginfo_class_MongoDB_BSON_Value_getValue, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_BSON_Value, fromPHP, arginfo_class_MongoDB_BSON_Value_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_BSON_Value, fromPHP, arginfo_class_MongoDB_BSON_Value_fromPHP, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_BSON_Value, isArray, arginfo_class_MongoDB_BSON_Value_isArray, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isBinary, arginfo_class_MongoDB_BSON_Value_isBinary, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isBool, arginfo_class_MongoDB_BSON_Value_isBool, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isCode, arginfo_class_MongoDB_BSON_Value_isCode, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isDBPointer, arginfo_class_MongoDB_BSON_Value_isDBPointer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isDecimal128, arginfo_class_MongoDB_BSON_Value_isDecimal128, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isDocument, arginfo_class_MongoDB_BSON_Value_isDocument, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isDouble, arginfo_class_MongoDB_BSON_Value_isDouble, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isInt, arginfo_class_MongoDB_BSON_Value_isInt, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isInt32, arginfo_class_MongoDB_BSON_Value_isInt32, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isInt64, arginfo_class_MongoDB_BSON_Value_isInt64, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isMaxKey, arginfo_class_MongoDB_BSON_Value_isMaxKey, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isMinKey, arginfo_class_MongoDB_BSON_Value_isMinKey, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isNumber, arginfo_class_MongoDB_BSON_Value_isNumber, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isNull, arginfo_class_MongoDB_BSON_Value_isNull, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isObjectId, arginfo_class_MongoDB_BSON_Value_isObjectId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isRegex, arginfo_class_MongoDB_BSON_Value_isRegex, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isSymbol, arginfo_class_MongoDB_BSON_Value_isSymbol, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isTimestamp, arginfo_class_MongoDB_BSON_Value_isTimestamp, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isUndefined, arginfo_class_MongoDB_BSON_Value_isUndefined, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isUTCDateTime, arginfo_class_MongoDB_BSON_Value_isUTCDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_BSON_Value, isUtf8, arginfo_class_MongoDB_BSON_Value_isUtf8, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_Value(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Value", class_MongoDB_BSON_Value_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zval const_TYPE_EOD_value;
	ZVAL_LONG(&const_TYPE_EOD_value, BSON_TYPE_EOD);
	zend_string *const_TYPE_EOD_name = zend_string_init_interned("TYPE_EOD", sizeof("TYPE_EOD") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_EOD_name, &const_TYPE_EOD_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_EOD_name);
	ZEND_ASSERT(BSON_TYPE_EOD == 0);

	zval const_TYPE_DOUBLE_value;
	ZVAL_LONG(&const_TYPE_DOUBLE_value, BSON_TYPE_DOUBLE);
	zend_string *const_TYPE_DOUBLE_name = zend_string_init_interned("TYPE_DOUBLE", sizeof("TYPE_DOUBLE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_DOUBLE_name, &const_TYPE_DOUBLE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_DOUBLE_name);
	ZEND_ASSERT(BSON_TYPE_DOUBLE == 1);

	zval const_TYPE_UTF8_value;
	ZVAL_LONG(&const_TYPE_UTF8_value, BSON_TYPE_UTF8);
	zend_string *const_TYPE_UTF8_name = zend_string_init_interned("TYPE_UTF8", sizeof("TYPE_UTF8") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_UTF8_name, &const_TYPE_UTF8_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_UTF8_name);
	ZEND_ASSERT(BSON_TYPE_UTF8 == 2);

	zval const_TYPE_DOCUMENT_value;
	ZVAL_LONG(&const_TYPE_DOCUMENT_value, BSON_TYPE_DOCUMENT);
	zend_string *const_TYPE_DOCUMENT_name = zend_string_init_interned("TYPE_DOCUMENT", sizeof("TYPE_DOCUMENT") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_DOCUMENT_name, &const_TYPE_DOCUMENT_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_DOCUMENT_name);
	ZEND_ASSERT(BSON_TYPE_DOCUMENT == 3);

	zval const_TYPE_ARRAY_value;
	ZVAL_LONG(&const_TYPE_ARRAY_value, BSON_TYPE_ARRAY);
	zend_string *const_TYPE_ARRAY_name = zend_string_init_interned("TYPE_ARRAY", sizeof("TYPE_ARRAY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_ARRAY_name, &const_TYPE_ARRAY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_ARRAY_name);
	ZEND_ASSERT(BSON_TYPE_ARRAY == 4);

	zval const_TYPE_BINARY_value;
	ZVAL_LONG(&const_TYPE_BINARY_value, BSON_TYPE_BINARY);
	zend_string *const_TYPE_BINARY_name = zend_string_init_interned("TYPE_BINARY", sizeof("TYPE_BINARY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_BINARY_name, &const_TYPE_BINARY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_BINARY_name);
	ZEND_ASSERT(BSON_TYPE_BINARY == 5);

	zval const_TYPE_UNDEFINED_value;
	ZVAL_LONG(&const_TYPE_UNDEFINED_value, BSON_TYPE_UNDEFINED);
	zend_string *const_TYPE_UNDEFINED_name = zend_string_init_interned("TYPE_UNDEFINED", sizeof("TYPE_UNDEFINED") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_UNDEFINED_name, &const_TYPE_UNDEFINED_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_UNDEFINED_name);
	ZEND_ASSERT(BSON_TYPE_UNDEFINED == 6);

	zval const_TYPE_OID_value;
	ZVAL_LONG(&const_TYPE_OID_value, BSON_TYPE_OID);
	zend_string *const_TYPE_OID_name = zend_string_init_interned("TYPE_OID", sizeof("TYPE_OID") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_OID_name, &const_TYPE_OID_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_OID_name);
	ZEND_ASSERT(BSON_TYPE_OID == 7);

	zval const_TYPE_BOOL_value;
	ZVAL_LONG(&const_TYPE_BOOL_value, BSON_TYPE_BOOL);
	zend_string *const_TYPE_BOOL_name = zend_string_init_interned("TYPE_BOOL", sizeof("TYPE_BOOL") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_BOOL_name, &const_TYPE_BOOL_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_BOOL_name);
	ZEND_ASSERT(BSON_TYPE_BOOL == 8);

	zval const_TYPE_DATE_TIME_value;
	ZVAL_LONG(&const_TYPE_DATE_TIME_value, BSON_TYPE_DATE_TIME);
	zend_string *const_TYPE_DATE_TIME_name = zend_string_init_interned("TYPE_DATE_TIME", sizeof("TYPE_DATE_TIME") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_DATE_TIME_name, &const_TYPE_DATE_TIME_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_DATE_TIME_name);
	ZEND_ASSERT(BSON_TYPE_DATE_TIME == 9);

	zval const_TYPE_NULL_value;
	ZVAL_LONG(&const_TYPE_NULL_value, BSON_TYPE_NULL);
	zend_string *const_TYPE_NULL_name = zend_string_init_interned("TYPE_NULL", sizeof("TYPE_NULL") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_NULL_name, &const_TYPE_NULL_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_NULL_name);
	ZEND_ASSERT(BSON_TYPE_NULL == 10);

	zval const_TYPE_REGEX_value;
	ZVAL_LONG(&const_TYPE_REGEX_value, BSON_TYPE_REGEX);
	zend_string *const_TYPE_REGEX_name = zend_string_init_interned("TYPE_REGEX", sizeof("TYPE_REGEX") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_REGEX_name, &const_TYPE_REGEX_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_REGEX_name);
	ZEND_ASSERT(BSON_TYPE_REGEX == 11);

	zval const_TYPE_DBPOINTER_value;
	ZVAL_LONG(&const_TYPE_DBPOINTER_value, BSON_TYPE_DBPOINTER);
	zend_string *const_TYPE_DBPOINTER_name = zend_string_init_interned("TYPE_DBPOINTER", sizeof("TYPE_DBPOINTER") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_DBPOINTER_name, &const_TYPE_DBPOINTER_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_DBPOINTER_name);
	ZEND_ASSERT(BSON_TYPE_DBPOINTER == 12);

	zval const_TYPE_CODE_value;
	ZVAL_LONG(&const_TYPE_CODE_value, BSON_TYPE_CODE);
	zend_string *const_TYPE_CODE_name = zend_string_init_interned("TYPE_CODE", sizeof("TYPE_CODE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_CODE_name, &const_TYPE_CODE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_CODE_name);
	ZEND_ASSERT(BSON_TYPE_CODE == 13);

	zval const_TYPE_SYMBOL_value;
	ZVAL_LONG(&const_TYPE_SYMBOL_value, BSON_TYPE_SYMBOL);
	zend_string *const_TYPE_SYMBOL_name = zend_string_init_interned("TYPE_SYMBOL", sizeof("TYPE_SYMBOL") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_SYMBOL_name, &const_TYPE_SYMBOL_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_SYMBOL_name);
	ZEND_ASSERT(BSON_TYPE_SYMBOL == 14);

	zval const_TYPE_CODEWSCOPE_value;
	ZVAL_LONG(&const_TYPE_CODEWSCOPE_value, BSON_TYPE_CODEWSCOPE);
	zend_string *const_TYPE_CODEWSCOPE_name = zend_string_init_interned("TYPE_CODEWSCOPE", sizeof("TYPE_CODEWSCOPE") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_CODEWSCOPE_name, &const_TYPE_CODEWSCOPE_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_CODEWSCOPE_name);
	ZEND_ASSERT(BSON_TYPE_CODEWSCOPE == 15);

	zval const_TYPE_INT32_value;
	ZVAL_LONG(&const_TYPE_INT32_value, BSON_TYPE_INT32);
	zend_string *const_TYPE_INT32_name = zend_string_init_interned("TYPE_INT32", sizeof("TYPE_INT32") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_INT32_name, &const_TYPE_INT32_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_INT32_name);
	ZEND_ASSERT(BSON_TYPE_INT32 == 16);

	zval const_TYPE_TIMESTAMP_value;
	ZVAL_LONG(&const_TYPE_TIMESTAMP_value, BSON_TYPE_TIMESTAMP);
	zend_string *const_TYPE_TIMESTAMP_name = zend_string_init_interned("TYPE_TIMESTAMP", sizeof("TYPE_TIMESTAMP") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_TIMESTAMP_name, &const_TYPE_TIMESTAMP_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_TIMESTAMP_name);
	ZEND_ASSERT(BSON_TYPE_TIMESTAMP == 17);

	zval const_TYPE_INT64_value;
	ZVAL_LONG(&const_TYPE_INT64_value, BSON_TYPE_INT64);
	zend_string *const_TYPE_INT64_name = zend_string_init_interned("TYPE_INT64", sizeof("TYPE_INT64") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_INT64_name, &const_TYPE_INT64_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_INT64_name);
	ZEND_ASSERT(BSON_TYPE_INT64 == 18);

	zval const_TYPE_DECIMAL128_value;
	ZVAL_LONG(&const_TYPE_DECIMAL128_value, BSON_TYPE_DECIMAL128);
	zend_string *const_TYPE_DECIMAL128_name = zend_string_init_interned("TYPE_DECIMAL128", sizeof("TYPE_DECIMAL128") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_DECIMAL128_name, &const_TYPE_DECIMAL128_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_DECIMAL128_name);
	ZEND_ASSERT(BSON_TYPE_DECIMAL128 == 19);

	zval const_TYPE_MAXKEY_value;
	ZVAL_LONG(&const_TYPE_MAXKEY_value, BSON_TYPE_MAXKEY);
	zend_string *const_TYPE_MAXKEY_name = zend_string_init_interned("TYPE_MAXKEY", sizeof("TYPE_MAXKEY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_MAXKEY_name, &const_TYPE_MAXKEY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_MAXKEY_name);
	ZEND_ASSERT(BSON_TYPE_MAXKEY == 127);

	zval const_TYPE_MINKEY_value;
	ZVAL_LONG(&const_TYPE_MINKEY_value, BSON_TYPE_MINKEY);
	zend_string *const_TYPE_MINKEY_name = zend_string_init_interned("TYPE_MINKEY", sizeof("TYPE_MINKEY") - 1, 1);
	zend_declare_class_constant_ex(class_entry, const_TYPE_MINKEY_name, &const_TYPE_MINKEY_value, ZEND_ACC_PUBLIC, NULL);
	zend_string_release(const_TYPE_MINKEY_name);
	ZEND_ASSERT(BSON_TYPE_MINKEY == 255);

	return class_entry;
}

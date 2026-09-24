/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 75d0ae9c2264b5e833f49af96ba474a64ce56bc3 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_WriteResult___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteResult_getInsertedCount, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_WriteResult_getMatchedCount arginfo_class_MongoDB_Driver_WriteResult_getInsertedCount

#define arginfo_class_MongoDB_Driver_WriteResult_getModifiedCount arginfo_class_MongoDB_Driver_WriteResult_getInsertedCount

#define arginfo_class_MongoDB_Driver_WriteResult_getDeletedCount arginfo_class_MongoDB_Driver_WriteResult_getInsertedCount

#define arginfo_class_MongoDB_Driver_WriteResult_getUpsertedCount arginfo_class_MongoDB_Driver_WriteResult_getInsertedCount

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_WriteResult_getServer, 0, 0, MongoDB\\Driver\\Server, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteResult_getUpsertedIds, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_WriteResult_getWriteConcernError, 0, 0, MongoDB\\Driver\\WriteConcernError, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_WriteResult_getWriteErrors arginfo_class_MongoDB_Driver_WriteResult_getUpsertedIds

#define arginfo_class_MongoDB_Driver_WriteResult_getErrorReplies arginfo_class_MongoDB_Driver_WriteResult_getUpsertedIds

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteResult_isAcknowledged, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

static ZEND_METHOD(MongoDB_Driver_WriteResult, __construct);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getInsertedCount);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getMatchedCount);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getModifiedCount);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getDeletedCount);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getUpsertedCount);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getServer);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getUpsertedIds);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getWriteConcernError);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getWriteErrors);
static ZEND_METHOD(MongoDB_Driver_WriteResult, getErrorReplies);
static ZEND_METHOD(MongoDB_Driver_WriteResult, isAcknowledged);

static const zend_function_entry class_MongoDB_Driver_WriteResult_methods[] = {
	ZEND_ME(MongoDB_Driver_WriteResult, __construct, arginfo_class_MongoDB_Driver_WriteResult___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getInsertedCount, arginfo_class_MongoDB_Driver_WriteResult_getInsertedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getMatchedCount, arginfo_class_MongoDB_Driver_WriteResult_getMatchedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getModifiedCount, arginfo_class_MongoDB_Driver_WriteResult_getModifiedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getDeletedCount, arginfo_class_MongoDB_Driver_WriteResult_getDeletedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getUpsertedCount, arginfo_class_MongoDB_Driver_WriteResult_getUpsertedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getServer, arginfo_class_MongoDB_Driver_WriteResult_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getUpsertedIds, arginfo_class_MongoDB_Driver_WriteResult_getUpsertedIds, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getWriteConcernError, arginfo_class_MongoDB_Driver_WriteResult_getWriteConcernError, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getWriteErrors, arginfo_class_MongoDB_Driver_WriteResult_getWriteErrors, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, getErrorReplies, arginfo_class_MongoDB_Driver_WriteResult_getErrorReplies, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteResult, isAcknowledged, arginfo_class_MongoDB_Driver_WriteResult_isAcknowledged, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_WriteResult(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteResult", class_MongoDB_Driver_WriteResult_methods);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	zval property_insertedCount_default_value;
	ZVAL_UNDEF(&property_insertedCount_default_value);
	zend_string *property_insertedCount_name = zend_string_init("insertedCount", sizeof("insertedCount") - 1, 1);
	zend_declare_typed_property(class_entry, property_insertedCount_name, &property_insertedCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release(property_insertedCount_name);

	zval property_matchedCount_default_value;
	ZVAL_UNDEF(&property_matchedCount_default_value);
	zend_string *property_matchedCount_name = zend_string_init("matchedCount", sizeof("matchedCount") - 1, 1);
	zend_declare_typed_property(class_entry, property_matchedCount_name, &property_matchedCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release(property_matchedCount_name);

	zval property_modifiedCount_default_value;
	ZVAL_UNDEF(&property_modifiedCount_default_value);
	zend_string *property_modifiedCount_name = zend_string_init("modifiedCount", sizeof("modifiedCount") - 1, 1);
	zend_declare_typed_property(class_entry, property_modifiedCount_name, &property_modifiedCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release(property_modifiedCount_name);

	zval property_deletedCount_default_value;
	ZVAL_UNDEF(&property_deletedCount_default_value);
	zend_string *property_deletedCount_name = zend_string_init("deletedCount", sizeof("deletedCount") - 1, 1);
	zend_declare_typed_property(class_entry, property_deletedCount_name, &property_deletedCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release(property_deletedCount_name);

	zval property_upsertedCount_default_value;
	ZVAL_UNDEF(&property_upsertedCount_default_value);
	zend_string *property_upsertedCount_name = zend_string_init("upsertedCount", sizeof("upsertedCount") - 1, 1);
	zend_declare_typed_property(class_entry, property_upsertedCount_name, &property_upsertedCount_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG|MAY_BE_NULL));
	zend_string_release(property_upsertedCount_name);

	zval property_server_default_value;
	ZVAL_UNDEF(&property_server_default_value);
	zend_string *property_server_name = zend_string_init("server", sizeof("server") - 1, 1);
	zend_string *property_server_class_MongoDB_Driver_Server = zend_string_init("MongoDB\\Driver\\Server", sizeof("MongoDB\\Driver\\Server")-1, 1);
	zend_declare_typed_property(class_entry, property_server_name, &property_server_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_server_class_MongoDB_Driver_Server, 0, 0));
	zend_string_release(property_server_name);

	zval property_upsertedIds_default_value;
	ZVAL_UNDEF(&property_upsertedIds_default_value);
	zend_string *property_upsertedIds_name = zend_string_init("upsertedIds", sizeof("upsertedIds") - 1, 1);
	zend_declare_typed_property(class_entry, property_upsertedIds_name, &property_upsertedIds_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release(property_upsertedIds_name);

	zval property_writeErrors_default_value;
	ZVAL_UNDEF(&property_writeErrors_default_value);
	zend_string *property_writeErrors_name = zend_string_init("writeErrors", sizeof("writeErrors") - 1, 1);
	zend_declare_typed_property(class_entry, property_writeErrors_name, &property_writeErrors_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release(property_writeErrors_name);

	zval property_writeConcernError_default_value;
	ZVAL_UNDEF(&property_writeConcernError_default_value);
	zend_string *property_writeConcernError_name = zend_string_init("writeConcernError", sizeof("writeConcernError") - 1, 1);
	zend_string *property_writeConcernError_class_MongoDB_Driver_WriteConcernError = zend_string_init("MongoDB\\Driver\\WriteConcernError", sizeof("MongoDB\\Driver\\WriteConcernError")-1, 1);
	zend_declare_typed_property(class_entry, property_writeConcernError_name, &property_writeConcernError_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_writeConcernError_class_MongoDB_Driver_WriteConcernError, 0, MAY_BE_NULL));
	zend_string_release(property_writeConcernError_name);

	zval property_writeConcern_default_value;
	ZVAL_UNDEF(&property_writeConcern_default_value);
	zend_string *property_writeConcern_name = zend_string_init("writeConcern", sizeof("writeConcern") - 1, 1);
	zend_string *property_writeConcern_class_MongoDB_Driver_WriteConcern = zend_string_init("MongoDB\\Driver\\WriteConcern", sizeof("MongoDB\\Driver\\WriteConcern")-1, 1);
	zend_declare_typed_property(class_entry, property_writeConcern_name, &property_writeConcern_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_writeConcern_class_MongoDB_Driver_WriteConcern, 0, MAY_BE_NULL));
	zend_string_release(property_writeConcern_name);

	zval property_errorReplies_default_value;
	ZVAL_UNDEF(&property_errorReplies_default_value);
	zend_string *property_errorReplies_name = zend_string_init("errorReplies", sizeof("errorReplies") - 1, 1);
	zend_declare_typed_property(class_entry, property_errorReplies_name, &property_errorReplies_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release(property_errorReplies_name);

	return class_entry;
}

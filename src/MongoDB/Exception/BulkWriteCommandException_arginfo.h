/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 16a2478ef423f897f914ad7b2ed3dbbdc4036e60 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getErrorReply, 0, 0, MongoDB\\BSON\\Document, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getPartialResult, 0, 0, MongoDB\\Driver\\BulkWriteCommandResult, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getWriteErrors, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getWriteConcernErrors arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getWriteErrors


static ZEND_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getErrorReply);
static ZEND_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getPartialResult);
static ZEND_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getWriteErrors);
static ZEND_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getWriteConcernErrors);


static const zend_function_entry class_MongoDB_Driver_Exception_BulkWriteCommandException_methods[] = {
	ZEND_ME(MongoDB_Driver_Exception_BulkWriteCommandException, getErrorReply, arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getErrorReply, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Exception_BulkWriteCommandException, getPartialResult, arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getPartialResult, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Exception_BulkWriteCommandException, getWriteErrors, arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getWriteErrors, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_Exception_BulkWriteCommandException, getWriteConcernErrors, arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getWriteConcernErrors, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_BulkWriteCommandException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ServerException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "BulkWriteCommandException", class_MongoDB_Driver_Exception_BulkWriteCommandException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ServerException);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zend_string *property_errorReply_class_MongoDB_BSON_Document = zend_string_init("MongoDB\\BSON\\Document", sizeof("MongoDB\\BSON\\Document")-1, 1);
	zval property_errorReply_default_value;
	ZVAL_NULL(&property_errorReply_default_value);
	zend_string *property_errorReply_name = zend_string_init("errorReply", sizeof("errorReply") - 1, 1);
	zend_declare_typed_property(class_entry, property_errorReply_name, &property_errorReply_default_value, ZEND_ACC_PRIVATE, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_errorReply_class_MongoDB_BSON_Document, 0, MAY_BE_NULL));
	zend_string_release(property_errorReply_name);

	zend_string *property_partialResult_class_MongoDB_Driver_BulkWriteCommandResult = zend_string_init("MongoDB\\Driver\\BulkWriteCommandResult", sizeof("MongoDB\\Driver\\BulkWriteCommandResult")-1, 1);
	zval property_partialResult_default_value;
	ZVAL_NULL(&property_partialResult_default_value);
	zend_string *property_partialResult_name = zend_string_init("partialResult", sizeof("partialResult") - 1, 1);
	zend_declare_typed_property(class_entry, property_partialResult_name, &property_partialResult_default_value, ZEND_ACC_PRIVATE, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_partialResult_class_MongoDB_Driver_BulkWriteCommandResult, 0, MAY_BE_NULL));
	zend_string_release(property_partialResult_name);

	zval property_writeErrors_default_value;
	ZVAL_EMPTY_ARRAY(&property_writeErrors_default_value);
	zend_string *property_writeErrors_name = zend_string_init("writeErrors", sizeof("writeErrors") - 1, 1);
	zend_declare_typed_property(class_entry, property_writeErrors_name, &property_writeErrors_default_value, ZEND_ACC_PRIVATE, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release(property_writeErrors_name);

	zval property_writeConcernErrors_default_value;
	ZVAL_EMPTY_ARRAY(&property_writeConcernErrors_default_value);
	zend_string *property_writeConcernErrors_name = zend_string_init("writeConcernErrors", sizeof("writeConcernErrors") - 1, 1);
	zend_declare_typed_property(class_entry, property_writeConcernErrors_name, &property_writeConcernErrors_default_value, ZEND_ACC_PRIVATE, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release(property_writeConcernErrors_name);

	return class_entry;
}

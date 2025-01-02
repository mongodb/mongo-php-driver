/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e56ba0ccf0a1c5ef736ef9778f05368b27f6febf */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getBulkWriteCommandResult, 0, 0, MongoDB\\Driver\\BulkWriteCommandResult, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getBulkWriteCommandResult);


static const zend_function_entry class_MongoDB_Driver_Exception_BulkWriteCommandException_methods[] = {
	ZEND_ME(MongoDB_Driver_Exception_BulkWriteCommandException, getBulkWriteCommandResult, arginfo_class_MongoDB_Driver_Exception_BulkWriteCommandException_getBulkWriteCommandResult, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_BulkWriteCommandException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ServerException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "BulkWriteCommandException", class_MongoDB_Driver_Exception_BulkWriteCommandException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ServerException);

	zval property_bulkWriteCommandResult_default_value;
	ZVAL_UNDEF(&property_bulkWriteCommandResult_default_value);
	zend_string *property_bulkWriteCommandResult_name = zend_string_init("bulkWriteCommandResult", sizeof("bulkWriteCommandResult") - 1, 1);
	zend_string *property_bulkWriteCommandResult_class_MongoDB_Driver_BulkWriteCommandResult = zend_string_init("MongoDB\\Driver\\BulkWriteCommandResult", sizeof("MongoDB\\Driver\\BulkWriteCommandResult")-1, 1);
	zend_declare_typed_property(class_entry, property_bulkWriteCommandResult_name, &property_bulkWriteCommandResult_default_value, ZEND_ACC_PROTECTED, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_bulkWriteCommandResult_class_MongoDB_Driver_BulkWriteCommandResult, 0, 0));
	zend_string_release(property_bulkWriteCommandResult_name);

	return class_entry;
}

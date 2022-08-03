/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 55cb6773eee137316b8b5a78cd1050b061b2e24e */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Exception_CommandException_getResultDocument, 0, 0, IS_OBJECT, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_Exception_CommandException, getResultDocument);


static const zend_function_entry class_MongoDB_Driver_Exception_CommandException_methods[] = {
	ZEND_ME(MongoDB_Driver_Exception_CommandException, getResultDocument, arginfo_class_MongoDB_Driver_Exception_CommandException_getResultDocument, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Exception_CommandException(zend_class_entry *class_entry_MongoDB_Driver_Exception_ServerException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "CommandException", class_MongoDB_Driver_Exception_CommandException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_MongoDB_Driver_Exception_ServerException);

	zval property_resultDocument_default_value;
	ZVAL_NULL(&property_resultDocument_default_value);
	zend_string *property_resultDocument_name = zend_string_init("resultDocument", sizeof("resultDocument") - 1, 1);
	zend_declare_property_ex(class_entry, property_resultDocument_name, &property_resultDocument_default_value, ZEND_ACC_PROTECTED, NULL);
	zend_string_release(property_resultDocument_name);

	return class_entry;
}

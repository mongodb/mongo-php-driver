/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a405afbb8eaa3bf544759f7a6ae9330ced7e7cc0 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getCode, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getInfo, 0, 0, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getMessage, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_WriteConcernError, __construct);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getCode);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getInfo);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getMessage);


static const zend_function_entry class_MongoDB_Driver_WriteConcernError_methods[] = {
	ZEND_ME(MongoDB_Driver_WriteConcernError, __construct, arginfo_class_MongoDB_Driver_WriteConcernError___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getCode, arginfo_class_MongoDB_Driver_WriteConcernError_getCode, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getInfo, arginfo_class_MongoDB_Driver_WriteConcernError_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getMessage, arginfo_class_MongoDB_Driver_WriteConcernError_getMessage, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_WriteConcernError(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcernError", class_MongoDB_Driver_WriteConcernError_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}

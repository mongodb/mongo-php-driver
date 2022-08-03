/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: fe3a956a163d5a46651f7ab5cb6be71552191a9d */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getCode, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getInfo, 0, 0, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError_getMessage, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteConcernError___wakeup, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_WriteConcernError, __construct);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getCode);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getInfo);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, getMessage);
static ZEND_METHOD(MongoDB_Driver_WriteConcernError, __wakeup);


static const zend_function_entry class_MongoDB_Driver_WriteConcernError_methods[] = {
	ZEND_ME(MongoDB_Driver_WriteConcernError, __construct, arginfo_class_MongoDB_Driver_WriteConcernError___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getCode, arginfo_class_MongoDB_Driver_WriteConcernError_getCode, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getInfo, arginfo_class_MongoDB_Driver_WriteConcernError_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, getMessage, arginfo_class_MongoDB_Driver_WriteConcernError_getMessage, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteConcernError, __wakeup, arginfo_class_MongoDB_Driver_WriteConcernError___wakeup, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_WriteConcernError(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcernError", class_MongoDB_Driver_WriteConcernError_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

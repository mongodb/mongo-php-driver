/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 8f9ebc299c90e86c6a55ea39e02d5a1734d10402 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_WriteError___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteError_getCode, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_MongoDB_Driver_WriteError_getIndex arginfo_class_MongoDB_Driver_WriteError_getCode

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteError_getInfo, 0, 0, IS_OBJECT, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_WriteError_getMessage, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()


static ZEND_METHOD(MongoDB_Driver_WriteError, __construct);
static ZEND_METHOD(MongoDB_Driver_WriteError, getCode);
static ZEND_METHOD(MongoDB_Driver_WriteError, getIndex);
static ZEND_METHOD(MongoDB_Driver_WriteError, getInfo);
static ZEND_METHOD(MongoDB_Driver_WriteError, getMessage);


static const zend_function_entry class_MongoDB_Driver_WriteError_methods[] = {
	ZEND_ME(MongoDB_Driver_WriteError, __construct, arginfo_class_MongoDB_Driver_WriteError___construct, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteError, getCode, arginfo_class_MongoDB_Driver_WriteError_getCode, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteError, getIndex, arginfo_class_MongoDB_Driver_WriteError_getIndex, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteError, getInfo, arginfo_class_MongoDB_Driver_WriteError_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_ME(MongoDB_Driver_WriteError, getMessage, arginfo_class_MongoDB_Driver_WriteError_getMessage, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_WriteError(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteError", class_MongoDB_Driver_WriteError_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE;

	return class_entry;
}

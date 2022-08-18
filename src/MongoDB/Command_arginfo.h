/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: e181b2dbea612c11c9318cc302901228e887c559 */

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Command___construct, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, document, MAY_BE_ARRAY|MAY_BE_OBJECT, NULL)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, commandOptions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

#if !(PHP_VERSION_ID >= 80000)
ZEND_BEGIN_ARG_INFO_EX(arginfo_class_MongoDB_Driver_Command___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, document)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, commandOptions, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_MongoDB_Driver_Command___wakeup, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()


#if PHP_VERSION_ID >= 80000
static ZEND_METHOD(MongoDB_Driver_Command, __construct);
#endif
#if !(PHP_VERSION_ID >= 80000)
static ZEND_METHOD(MongoDB_Driver_Command, __construct);
#endif
static ZEND_METHOD(MongoDB_Driver_Command, __wakeup);


static const zend_function_entry class_MongoDB_Driver_Command_methods[] = {
#if PHP_VERSION_ID >= 80000
	ZEND_ME(MongoDB_Driver_Command, __construct, arginfo_class_MongoDB_Driver_Command___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
#if !(PHP_VERSION_ID >= 80000)
	ZEND_ME(MongoDB_Driver_Command, __construct, arginfo_class_MongoDB_Driver_Command___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	ZEND_ME(MongoDB_Driver_Command, __wakeup, arginfo_class_MongoDB_Driver_Command___wakeup, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_Driver_Command(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Command", class_MongoDB_Driver_Command_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	return class_entry;
}

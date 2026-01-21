/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 9fc1377046ca92a6a50cfac11fabc50b4d30d708 */




static const zend_function_entry class_MongoDB_BSON_VectorType_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_MongoDB_BSON_VectorType(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("MongoDB\\BSON\\VectorType", IS_UNDEF, class_MongoDB_BSON_VectorType_methods);

	zend_enum_add_case_cstr(class_entry, "Float32", NULL);

	zend_enum_add_case_cstr(class_entry, "Int8", NULL);

	zend_enum_add_case_cstr(class_entry, "PackedBit", NULL);

	return class_entry;
}

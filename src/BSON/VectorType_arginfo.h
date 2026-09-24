/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f6db546bc317d0d1ab8e7a80ecaf92a058f49875 */

static zend_class_entry *register_class_MongoDB_BSON_VectorType(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("MongoDB\\BSON\\VectorType", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Float32", NULL);

	zend_enum_add_case_cstr(class_entry, "Int8", NULL);

	zend_enum_add_case_cstr(class_entry, "PackedBit", NULL);

	return class_entry;
}

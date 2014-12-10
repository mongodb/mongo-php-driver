<?php
namespace BSON {
    interface Type {}

    class ObjectID implements Type {

        /**
         * Constructs a new Object ID, optionally from a string
         *
         * @param string $id The String to make into BSON ID type
         */
        function __construct($id = null) {
        /*** CIMPL ***/
/*
	if (ZEND_NUM_ARGS()) {
		if (bson_oid_is_valid(id, id_len)) {
			intern->oid = ecalloc(1, sizeof(bson_oid_t));
			bson_oid_init_from_string(intern->oid, id);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid BSON ID provided" TSRMLS_CC);
		}
	} else {
		intern->oid = ecalloc(1, sizeof(bson_oid_t));
		bson_oid_init(intern->oid, NULL);
	}
*/
        /*** CIMPL ***/
        }
        function __toString() {
        /*** CEF ***/
/*
	char         id[25];
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	bson_oid_to_string(intern->oid, id);
	RETURN_STRINGL(id, 24, 1);
*/
        /*** CIMPL ***/
        }
    }
    class Binary implements Type {
        const TYPE_GENERIC      = 0x00;
        const TYPE_FUNCTION     = 0x01;
        const TYPE_OLD_BINARY   = 0x02;
        const TYPE_OLD_UUID     = 0x03;
        const TYPE_UUID         = 0x04;
        const TYPE_MD5          = 0x05;
        const TYPE_USER_DEFINED = 0x80;

        function getSubType() {}
    }
    class Javascript implements Type {}

    class Regex implements Type {
        /**
         * Constructs a new regular expression.
         *
         * @param string $pattern
         * @param string $flags
         */
        function __construct($pattern, $flags) {
        /*** CIMPL ***/
/*
    intern->pattern = estrndup(pattern, pattern_len);
    intern->pattern_len = pattern_len;
    intern->flags = estrndup(flags, flags_len);
    intern->flags_len = flags_len;
*/
        /*** CIMPL ***/
        }

        public function getPattern()
        {
            /*** CIMPL ***/
    /*
        RETURN_STRINGL(intern->pattern, intern->pattern_len, 1);
    */
            /*** CIMPL ***/
        }

        public function getFlags()
        {
            /*** CIMPL ***/
    /*
        RETURN_STRINGL(intern->flags, intern->flags_len, 1);
    */
            /*** CIMPL ***/
        }

        function __toString() {
        /*** CEF ***/
/*
    char *regex;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
    spprintf(&regex, 0, "/%s/%s", intern->pattern, intern->flags);
    RETVAL_STRING(regex, 0);
*/
        /*** CIMPL ***/
        }
    }

    class UTCDatetime implements Type {}
    class DBRef implements Type {}
    class Timestamp implements Type {}
    class Int32 implements Type {}
    class Int64 implements Type {}
    class Log implements Type {}
    class MaxKey implements Type {}
    class MinKey implements Type {}


    function fromArray($arrayOrObject) {}
    function toArray($bson) {}
    function toJSON($bson, $extendedJSON = false) {}
    function fromJSON($json) {}

$ObjectID["funcs"] = <<< EOT
zend_object_handlers* php_phongo_handlers_objectid() /* {{{ */
{
	return &php_phongo_handler_objectid;
} /* }}} */
static int php_phongo_objectid_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
	php_phongo_objectid_t *intern1;
	php_phongo_objectid_t *intern2;

	intern1 = (php_phongo_objectid_t *)zend_object_store_get_object(o1 TSRMLS_CC);
	intern2 = (php_phongo_objectid_t *)zend_object_store_get_object(o2 TSRMLS_CC);

	return bson_oid_compare(intern1->oid, intern2->oid);
} /* }}} */

EOT;

$ObjectID["forward_declarations"] = <<< EOT

zend_object_handlers php_phongo_handler_objectid;


EOT;

$ObjectID["handlers_callback"] = "php_phongo_handlers_objectid";
$ObjectID["handlers_init"]     = "
	memcpy(&php_phongo_handler_objectid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_objectid.compare_objects = php_phongo_objectid_compare_objects;
";

$Regex["free"] = <<< EOF
    if (intern->pattern) {
        efree(intern->pattern);
    }

    if (intern->flags) {
        efree(intern->flags);
    }

EOF;

}

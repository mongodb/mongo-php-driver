<?php

namespace MongoDB;

/**
 * Value object for a write concern error.
 */
final class WriteConcernError
{
    /**
     * Returns the MongoDB error code
     *
     * @return integer Server error code
     */
    public function getCode()
    {
        /*** CIMPL ***/
/*
    RETURN_LONG(intern->code);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns additional metadata for the error
     *
     * @return array Additional metadata for the error (e.g. {"wtimeout": true})
     */
    public function getInfo()
    {
        /*** CIMPL ***/
/*
    if (intern->info && Z_TYPE_P(intern->info) == IS_ARRAY) {
        RETURN_ZVAL(intern->info, 1, 0);
    }

    array_init(return_value);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the actual error message from the server
     *
     * @return string Server error message
     */
    public function getMessage()
    {
/*
    RETURN_STRING(intern->message, 1);
*/
    }
}

$WriteConcernError["forward_declarations"] = <<< EOF
inline int writeconcernerror_populate(php_phongo_writeconcernerror_t *intern, bson_t *document);

EOF;

$WriteConcernError["funcs"] = <<< EOF
inline int writeconcernerror_populate(php_phongo_writeconcernerror_t *intern, bson_t *document) /* {{{ */
{
    bson_iter_t iter;

    if (bson_iter_init_find(&iter, document, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
        intern->code = bson_iter_int32(&iter);
    }

    if (bson_iter_init_find(&iter, document, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) {
        intern->message = bson_iter_dup_utf8(&iter, NULL);
    }

    if (bson_iter_init_find(&iter, document, "errInfo") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
        uint32_t len;
        const uint8_t *data;

        MAKE_STD_ZVAL(intern->info);
        bson_iter_document(&iter, &len, &data);

        if (!data) {
            return false;
        }

        if (!bson_to_zval(data, len, intern->info)) {
            zval_ptr_dtor(&intern->info);
            intern->info = NULL;

            return false;
        }
    }

    return true;
} /* }}} */

EOF;

$WriteConcernError["free"] = <<< EOF
    if (intern->message) {
        efree(intern->message);
    }

    if (intern->info) {
        zval_ptr_dtor(&intern->info);
    }

EOF;

<?php
namespace BSON {
    interface Type {}

    class ObjectID implements Type {
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
    class Regex implements Type {}
    class UTCDatetime implements Type {}
    class DBRef implements Type {}
    class Timestamp implements Type {}
    class Int32 implements Type {}
    class Int64 implements Type {}
    class Log implements Type {}
    class Pool implements Type {}
    class MaxKey implements Type {}
    class MinKey implements Type {}


    function encode($arrayOrObject) {}
    function decode($bson) {}
    function toJSON($bson, $extendedJSON = false) {}
}

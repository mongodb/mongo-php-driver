<?php
namespace BSON {

    class ObjectID {}
    class Binary {
        const TYPE_GENERIC      = 0x00;
        const TYPE_FUNCTION     = 0x01;
        const TYPE_OLD_BINARY   = 0x02;
        const TYPE_OLD_UUID     = 0x03;
        const TYPE_UUID         = 0x04;
        const TYPE_MD5          = 0x05;
        const TYPE_USER_DEFINED = 0x80;

        function getSubType() {}
    }
    class Javascript {}
    class Regex {}
    class UTCDatetime {}
    class DBRef {}
    class Timestamp {}
    class Int32 {}
    class Int64 {}
    class Log {}
    class Pool {}
    class MaxKey {}
    class MinKey {}


    function encode($arrayOrObject) {}
    function decode($bson) {}
    function toJSON($bson, $extendedJSON = false) {}
}



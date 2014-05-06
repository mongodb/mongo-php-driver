<?php
/* C interfaces */
namespace BSON\Types {
    interface Object {
    }

    interface ArrayObj {
    }

    interface ObjectID {
    }

    interface Binary {
        const TYPE_GENERIC      = 0x00;
        const TYPE_FUNCTION     = 0x01;
        const TYPE_OLD_BINARY   = 0x02;
        const TYPE_OLD_UUID     = 0x03;
        const TYPE_UUID         = 0x04;
        const TYPE_MD5          = 0x05;
        const TYPE_USER_DEFINED = 0x80;

        function getSubType();
    }

    interface DateTime {
    }
}

/* C objects */
namespace BSON {

    class Object implements \BSON\Types\Object {
    }

    class ArrayDynamic extends \ArrayObject implements \BSON\Types\ArrayObj {
    }

    class ArrayFixed extends \SplFixedArray implements \BSON\Types\ArrayObj {
    }

    class ObjectID implements \BSON\Types\ObjectID {
    }

    class DateTime extends \DateTime implements \BSON\Types\DateTime {
    }



    function encode($arrayOrObject) {
        /* Scalar not allowed */
    }

    function decode($bson) {
    }

    function toJSON($bson, $extendedJSON = false) {
    }
}




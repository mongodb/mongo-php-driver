<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Value
{
    /**
     * @var int
     * @cvalue BSON_TYPE_EOD
     */
    public const TYPE_EOD = 0x00;

    /**
     * @var int
     * @cvalue BSON_TYPE_DOUBLE
     */
    public const TYPE_DOUBLE = 0x01;

    /**
     * @var int
     * @cvalue BSON_TYPE_UTF8
     */
    public const TYPE_UTF8 = 0x02;

    /**
     * @var int
     * @cvalue BSON_TYPE_DOCUMENT
     */
    public const TYPE_DOCUMENT = 0x03;

    /**
     * @var int
     * @cvalue BSON_TYPE_ARRAY
     */
    public const TYPE_ARRAY = 0x04;

    /**
     * @var int
     * @cvalue BSON_TYPE_BINARY
     */
    public const TYPE_BINARY = 0x05;

    /**
     * @var int
     * @cvalue BSON_TYPE_UNDEFINED
     */
    public const TYPE_UNDEFINED = 0x06;

    /**
     * @var int
     * @cvalue BSON_TYPE_OID
     */
    public const TYPE_OID = 0x07;

    /**
     * @var int
     * @cvalue BSON_TYPE_BOOL
     */
    public const TYPE_BOOL = 0x08;

    /**
     * @var int
     * @cvalue BSON_TYPE_DATE_TIME
     */
    public const TYPE_DATE_TIME = 0x09;

    /**
     * @var int
     * @cvalue BSON_TYPE_NULL
     */
    public const TYPE_NULL = 0x0A;

    /**
     * @var int
     * @cvalue BSON_TYPE_REGEX
     */
    public const TYPE_REGEX = 0x0B;

    /**
     * @var int
     * @cvalue BSON_TYPE_DBPOINTER
     */
    public const TYPE_DBPOINTER = 0x0C;

    /**
     * @var int
     * @cvalue BSON_TYPE_CODE
     */
    public const TYPE_CODE = 0x0D;

    /**
     * @var int
     * @cvalue BSON_TYPE_SYMBOL
     */
    public const TYPE_SYMBOL = 0x0E;

    /**
     * @var int
     * @cvalue BSON_TYPE_CODEWSCOPE
     */
    public const TYPE_CODEWSCOPE = 0x0F;

    /**
     * @var int
     * @cvalue BSON_TYPE_INT32
     */
    public const TYPE_INT32 = 0x10;

    /**
     * @var int
     * @cvalue BSON_TYPE_TIMESTAMP
     */
    public const TYPE_TIMESTAMP = 0x11;

    /**
     * @var int
     * @cvalue BSON_TYPE_INT64
     */
    public const TYPE_INT64 = 0x12;

    /**
     * @var int
     * @cvalue BSON_TYPE_DECIMAL128
     */
    public const TYPE_DECIMAL128 = 0x13;

    /**
     * @var int
     * @cvalue BSON_TYPE_MAXKEY
     */
    public const TYPE_MAXKEY = 0x7F;

    /**
     * @var int
     * @cvalue BSON_TYPE_MINKEY
     */
    public const TYPE_MINKEY = 0xFF;

    private function __construct() {}

    final public function getType(): int {}

#if PHP_VERSION_ID >= 80000
    final public function getValue(): mixed {}
#else
    /** @return mixed */
    final public function getValue() {}
#endif

#if PHP_VERSION_ID >= 80000
    final static public function fromPHP(mixed $value): Value {}
#else
    /** @param mixed $value */
    final static public function fromPHP($value): Value {}
#endif

    /** Type checkers */
//    final public function isArray(): bool {}
//    final public function isDocument(): bool {}
//    final public function isInt(): bool {}
//    final public function isInt32(): bool {}
//    final public function isInt64(): bool {}
//    final public function isNumber(): bool {}
    // TODO: Other types

    /** @throws UnexpectedValueException */
//    final public function getArray(): PackedArray {}
//    final public function getDocument(): Document {}
//    final public function getInt(): int {}
//    final public function getInt32(): int {}
//    final public function getInt64(): int|Int64 {}
//    final public function getNumber(): int|float {}
//    final public function getObjectId(): ObjectId {}
//    final public function getUTCDateTime(): UTCDateTime {}
    // TODO: Other types
}
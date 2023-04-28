<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Value implements \Serializable
{
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

    /** Accessors */
    final public function getArray(): PackedArray {}
    final public function getBinary(): Binary {}
    final public function getBool(): bool {}
    final public function getCode(): Javascript {}
    final public function getDateTime(): UTCDateTime {}
    final public function getDBPointer(): DBPointer {}
    final public function getDecimal128(): Decimal128 {}
    final public function getDocument(): Document {}
    final public function getDouble(): float {}
    final public function getInt(): int {}
    final public function getInt32(): int {}
#if PHP_VERSION_ID >= 80000
    final public function getInt64(): int|Int64 {}
#else
    /** @return int|Int64 */
    final public function getInt64() {}
#endif
    final public function getMaxKey(): MaxKey {}
    final public function getMinKey(): MinKey {}
    final public function getNull(): null {}
    final public function getObjectId(): ObjectId {}
    final public function getRegex(): Regex {}
    final public function getSymbol(): Symbol {}
    final public function getTimestamp(): Timestamp {}
    final public function getUndefined(): Undefined {}
    final public function getUtf8(): string {}

    /** Type checkers */
    final public function isArray(): bool {}
    final public function isBinary(): bool {}
    final public function isBool(): bool {}
    final public function isCode(): bool {}
    final public function isDateTime(): bool {}
    final public function isDBPointer(): bool {}
    final public function isDecimal128(): bool {}
    final public function isDocument(): bool {}
    final public function isDouble(): bool {}
    final public function isInt(): bool {}
    final public function isInt32(): bool {}
    final public function isInt64(): bool {}
    final public function isMaxKey(): bool {}
    final public function isMinKey(): bool {}
    final public function isNull(): bool {}
    final public function isObjectId(): bool {}
    final public function isRegex(): bool {}
    final public function isSymbol(): bool {}
    final public function isTimestamp(): bool {}
    final public function isUndefined(): bool {}
    final public function isUtf8(): bool {}

    final public static function __set_state(array $properties): Regex {}

    final public function serialize(): string {}

    /** @param string $serialized */
    final public function unserialize($serialized): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}

<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Binary implements BinaryInterface, \JsonSerializable, Type, \Serializable
{
    /**
     * @var int
     * @cvalue BSON_SUBTYPE_BINARY
     */
    public const TYPE_GENERIC = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_FUNCTION
     */
    public const TYPE_FUNCTION = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_BINARY_DEPRECATED
     */
    public const TYPE_OLD_BINARY = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_UUID_DEPRECATED
     */
    public const TYPE_OLD_UUID = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_UUID
     */
    public const TYPE_UUID = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_MD5
     */
    public const TYPE_MD5 = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_ENCRYPTED
     */
    public const TYPE_ENCRYPTED = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_COLUMN
     */
    public const TYPE_COLUMN = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_SENSITIVE
     */
    public const TYPE_SENSITIVE = UNKNOWN;

    /**
     * @var int
     * @cvalue BSON_SUBTYPE_USER
     */
    public const TYPE_USER_DEFINED = UNKNOWN;

    final public function __construct(string $data, int $type = Binary::TYPE_GENERIC) {}

    final public function getData(): string {}

    final public function getType(): int {}

    final public static function __set_state(array $properties): Binary {}

    final public function __toString(): string {}

    final public function serialize(): string {}

#if PHP_VERSION_ID >= 80000
    final public function unserialize(string $data): void {}
#else
    /** @param string $serialized */
    final public function unserialize($serialized): void {}
#endif

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

#if PHP_VERSION_ID >= 80000
    final public function jsonSerialize(): mixed {}
#else
    /** @return mixed */
    final public function jsonSerialize() {}
#endif
}

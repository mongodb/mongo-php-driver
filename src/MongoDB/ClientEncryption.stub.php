<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class ClientEncryption
{
    /**
     * @var string
     * @cvalue MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC
     */
    public const AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM
     */
    public const AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_ENCRYPT_ALGORITHM_INDEXED
     */
    public const ALGORITHM_INDEXED = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_ENCRYPT_ALGORITHM_UNINDEXED
     */
    public const ALGORITHM_UNINDEXED = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_ENCRYPT_ALGORITHM_RANGEPREVIEW
     */
    public const ALGORITHM_RANGE_PREVIEW = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_ENCRYPT_QUERY_TYPE_EQUALITY
     */
    public const QUERY_TYPE_EQUALITY = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_ENCRYPT_QUERY_TYPE_RANGEPREVIEW
     */
    public const QUERY_TYPE_RANGE_PREVIEW = UNKNOWN;

    final public function __construct(array $options) {}

    final public function addKeyAltName(\MongoDB\BSON\Binary $keyId, string $keyAltName): ?object {}

    final public function createDataKey(string $kmsProvider, ?array $options = null): \MongoDB\BSON\Binary {}

#if PHP_VERSION_ID >= 80000
    final public function decrypt(\MongoDB\BSON\Binary $value): mixed {}
#else
    /** @return mixed */
    final public function decrypt(\MongoDB\BSON\Binary $value) {}
#endif

    final public function deleteKey(\MongoDB\BSON\Binary $keyId): object {}

#if PHP_VERSION_ID >= 80000
    final public function encrypt(mixed $value, ?array $options = null): \MongoDB\BSON\Binary {}
#else
    /** @param mixed $value */
    final public function encrypt($value, ?array $options = null): \MongoDB\BSON\Binary {}
#endif

#if PHP_VERSION_ID >= 80000
    final public function encryptExpression(array|object $expr, ?array $options = null): object {}
#else
    /** @param array|object $expr */
    final public function encryptExpression($expr, ?array $options = null): object {}
#endif

    final public function getKey(\MongoDB\BSON\Binary $keyId): ?object {}

    final public function getKeyByAltName(string $keyAltName): ?object {}

    final public function getKeys(): \MongoDB\Driver\Cursor {}

    final public function removeKeyAltName(\MongoDB\BSON\Binary $keyId, string $keyAltName): ?object {}

#if PHP_VERSION_ID >= 80000
    final public function rewrapManyDataKey(array|object $filter, ?array $options = null): object {}
#else
    /** @param array|object $filter */
    final public function rewrapManyDataKey($filter, ?array $options = null): object {}
#endif
}

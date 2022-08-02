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
     * @cvalue MONGOC_ENCRYPT_QUERY_TYPE_EQUALITY
     */
    public const QUERY_TYPE_EQUALITY = UNKNOWN;

    final public function __construct(array $options) {}

    final public function createDataKey(string $kmsProvider, array $options = []): \MongoDB\BSON\Binary {}

#if PHP_VERSION_ID >= 80000
    final public function decrypt(\MongoDB\BSON\Binary $value): mixed {}

    final public function encrypt(mixed $value, array $options = []): \MongoDB\BSON\Binary {}
#else
    /** @return mixed */
    final public function decrypt(\MongoDB\BSON\Binary $value) {}

    /** @param mixed $value */
    final public function encrypt($value, array $options = []): \MongoDB\BSON\Binary {}
#endif
}

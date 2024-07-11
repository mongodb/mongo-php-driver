<?php

/** @generate-function-entries */

namespace MongoDB\BSON {
    /** @deprecated use MongoDB\BSON\Document::fromJSON instead */
    function fromJSON(string $json): string {}

#if PHP_VERSION_ID >= 80000
    /** @deprecated use MongoDB\BSON\Document::fromPHP instead */
    function fromPHP(array|object $value): string {}
#else
    /**
     * @param array|object $value
     * @deprecated use MongoDB\BSON\Document::fromPHP instead
     */
    function fromPHP($value): string {}
#endif

    /** @deprecated use MongoDB\BSON\Document::toCanonicalExtendedJSON instead */
    function toCanonicalExtendedJSON(string $bson): string {}

    /** @deprecated */
    function toJSON(string $bson): string {}

#if PHP_VERSION_ID >= 80000
    /** @deprecated use MongoDB\BSON\Document::toPHP instead */
    function toPHP(string $bson, ?array $typemap = null): array|object {}
#else
    /**
     * @return array|object
     * @deprecated use MongoDB\BSON\Document::toPHP instead
     */
    function toPHP(string $bson, ?array $typemap = null) {}
#endif

    /** @deprecated use MongoDB\BSON\Document::toRelaxedExtendedJSON instead */
    function toRelaxedExtendedJSON(string $bson): string {}
}

namespace MongoDB\Driver\Monitoring {
    function addSubscriber(Subscriber $subscriber): void {}

    /** @internal */
    function mongoc_log(int $level, string $domain, string $message): void {}

    function removeSubscriber(Subscriber $subscriber): void {}
}

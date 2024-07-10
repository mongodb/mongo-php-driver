<?php

/** @generate-function-entries */

namespace MongoDB\BSON {
    /** @deprecated */
    function fromJSON(string $json): string {}

#if PHP_VERSION_ID >= 80000
    /** @deprecated */
    function fromPHP(array|object $value): string {}
#else
    /**
     * @param array|object $value
     * @deprecated
     */
    function fromPHP($value): string {}
#endif

    /** @deprecated */
    function toCanonicalExtendedJSON(string $bson): string {}

    /** @deprecated */
    function toJSON(string $bson): string {}

#if PHP_VERSION_ID >= 80000
    /** @deprecated */
    function toPHP(string $bson, ?array $typemap = null): array|object {}
#else
    /**
     * @return array|object
     * @deprecated
     */
    function toPHP(string $bson, ?array $typemap = null) {}
#endif

    /** @deprecated */
    function toRelaxedExtendedJSON(string $bson): string {}
}

namespace MongoDB\Driver\Monitoring {
    function addSubscriber(Subscriber $subscriber): void {}

    /** @internal */
    function mongoc_log(int $level, string $domain, string $message): void {}

    function removeSubscriber(Subscriber $subscriber): void {}
}

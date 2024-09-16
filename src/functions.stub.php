<?php

/** @generate-function-entries */

namespace MongoDB\BSON {
    /** @deprecated use MongoDB\BSON\Document::fromJSON instead */
    function fromJSON(string $json): string {}

    /** @deprecated use MongoDB\BSON\Document::fromPHP instead */
    function fromPHP(array|object $value): string {}

    /** @deprecated use MongoDB\BSON\Document::toCanonicalExtendedJSON instead */
    function toCanonicalExtendedJSON(string $bson): string {}

    /** @deprecated */
    function toJSON(string $bson): string {}

    /** @deprecated use MongoDB\BSON\Document::toPHP instead */
    function toPHP(string $bson, ?array $typemap = null): array|object {}

    /** @deprecated use MongoDB\BSON\Document::toRelaxedExtendedJSON instead */
    function toRelaxedExtendedJSON(string $bson): string {}
}

namespace MongoDB\Driver\Monitoring {
    function addSubscriber(Subscriber $subscriber): void {}

    /** @internal */
    function mongoc_log(int $level, string $domain, string $message): void {}

    function removeSubscriber(Subscriber $subscriber): void {}
}

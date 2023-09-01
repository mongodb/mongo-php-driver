<?php

/** @generate-function-entries */

namespace MongoDB\BSON {
    function fromJSON(string $json): string {}

#if PHP_VERSION_ID >= 80000
    function fromPHP(array|object $value): string {}
#else
    /** @param array|object $value */
    function fromPHP($value): string {}
#endif

    function toCanonicalExtendedJSON(string $bson): string {}

    function toJSON(string $bson): string {}

#if PHP_VERSION_ID >= 80000
    function toPHP(string $bson, ?array $typemap = null): array|object {}
#else
    /** @return array|object */
    function toPHP(string $bson, ?array $typemap = null) {}
#endif

    function toRelaxedExtendedJSON(string $bson): string {}
}

namespace MongoDB\Driver\Logging {
    function addLogger(Logger $logger): void {}

    function levelToString(int $level): string {}

    function log(int $level, string $message): void {}

    function removeLogger(Logger $logger): void {}
}

namespace MongoDB\Driver\Monitoring {
    function addSubscriber(Subscriber $subscriber): void {}

    function removeSubscriber(Subscriber $subscriber): void {}
}

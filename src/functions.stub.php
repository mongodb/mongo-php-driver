<?php

/**
 * @generate-function-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring {
    function addSubscriber(Subscriber $subscriber): void {}

    /** @internal */
    function mongoc_log(int $level, string $domain, string $message): void {}

    function removeSubscriber(Subscriber $subscriber): void {}
}

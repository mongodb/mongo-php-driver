<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver\Monitoring;

interface CommandSubscriber extends Subscriber
{
    /** @tentative-return-type */
    public function commandStarted(CommandStartedEvent $event): void;

    /** @tentative-return-type */
    public function commandSucceeded(CommandSucceededEvent $event): void;

    /** @tentative-return-type */
    public function commandFailed(CommandFailedEvent $event): void;
}

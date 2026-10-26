<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver\Monitoring;

interface CommandSubscriber extends Subscriber
{
    public function commandStarted(CommandStartedEvent $event): void;

    public function commandSucceeded(CommandSucceededEvent $event): void;

    public function commandFailed(CommandFailedEvent $event): void;
}

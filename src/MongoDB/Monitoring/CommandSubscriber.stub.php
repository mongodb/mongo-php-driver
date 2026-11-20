<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring;

interface CommandSubscriber extends Subscriber
{
    public function commandStarted(CommandStartedEvent $event): void;

    public function commandSucceeded(CommandSucceededEvent $event): void;

    public function commandFailed(CommandFailedEvent $event): void;
}

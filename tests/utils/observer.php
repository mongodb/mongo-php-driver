<?php
use MongoDB\Driver\Monitoring\CommandFailedEvent;
use MongoDB\Driver\Monitoring\CommandStartedEvent;
use MongoDB\Driver\Monitoring\CommandSucceededEvent;
use MongoDB\Driver\Monitoring\CommandSubscriber;
/**
 * Observes command documents using the driver's monitoring API.
 */
class CommandObserver implements CommandSubscriber
{
    private $commands = [];
    public function observe(callable $execution, callable $commandCallback)
    {
        $this->commands = [];
        \MongoDB\Driver\Monitoring\addSubscriber($this);
        try {
            call_user_func($execution);
        } finally {
            \MongoDB\Driver\Monitoring\removeSubscriber($this);
            foreach ($this->commands as $command) {
                call_user_func($commandCallback, $command);
            }
        }
    }
    public function commandStarted(CommandStartedEvent $event): void
    {
        $this->commands[] = $event->getCommand();
    }
    public function commandSucceeded(CommandSucceededEvent $event): void
    {
    }
    public function commandFailed(CommandFailedEvent $event): void
    {
    }
}
?>

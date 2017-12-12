--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = new MongoDB\Driver\Manager(STANDALONE);

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event )
    {
        echo "started: ", $event->getCommandName(), "\n";
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event )
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event )
    {
        echo "failed: ", $event->getCommandName(), "\n";
        echo "- getError() returns an object: ", is_object( $event->getError() ) ? 'yes' : 'no', "\n";
        echo "- getError() returns an MongoDB\Driver\Exception\Exception object: ", $event->getError() instanceof MongoDB\Driver\Exception\Exception ? 'yes' : 'no', "\n";
        echo "- getDurationMicros() returns an integer: ", is_integer( $event->getDurationMicros() ) ? 'yes' : 'no', "\n";
        echo "- getDurationMicros() returns > 0: ", $event->getDurationMicros() > 0 ? 'yes' : 'no', "\n";
        echo "- getCommandName() returns a string: ", is_string( $event->getCommandName() ) ? 'yes' : 'no', "\n";
        echo "- getCommandName() returns '", $event->getCommandName(), "'\n";
        echo "- getServer() returns an object: ", is_object( $event->getServer() ) ? 'yes' : 'no', "\n";
        echo "- getServer() returns a Server object: ", $event->getServer() instanceof MongoDB\Driver\Server ? 'yes' : 'no', "\n";
        echo "- getOperationId() returns a string: ", is_string( $event->getOperationId() ) ? 'yes' : 'no', "\n";
        echo "- getRequestId() returns a string: ", is_string( $event->getRequestId() ) ? 'yes' : 'no', "\n";
    }
}

$subscriber = new MySubscriber;

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

CLEANUP( STANDALONE );
?>
--EXPECT--
started: drop
failed: drop
- getError() returns an object: yes
- getError() returns an MongoDB\Driver\Exception\Exception object: yes
- getDurationMicros() returns an integer: yes
- getDurationMicros() returns > 0: yes
- getCommandName() returns a string: yes
- getCommandName() returns 'drop'
- getServer() returns an object: yes
- getServer() returns a Server object: yes
- getOperationId() returns a string: yes
- getRequestId() returns a string: yes

--TEST--
MongoDB\Driver\Monitoring\CommandSucceededEvent: requestId and operationId match
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $startRequestId;
    private $startOperationId;

    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event ): void
    {
        echo "started: ", $event->getCommandName(), "\n";
        $this->startRequestId = $event->getRequestId();
        $this->startOperationId = $event->getOperationId();
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event ): void
    {
        echo "succeeded: ", $event->getCommandName(), "\n";
        echo "- requestId matches: ", $this->startRequestId == $event->getRequestId() ? 'yes' : 'no', " \n";
        echo "- operationId matches: ", $this->startOperationId == $event->getOperationId() ? 'yes' : 'no', " \n";
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event ): void
    {
    }
}

$query = new MongoDB\Driver\Query( [] );
$subscriber = new MySubscriber;

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

$cursor = $m->executeQuery( "demo.test", $query );
?>
--EXPECT--
started: find
succeeded: find
- requestId matches: yes 
- operationId matches: yes

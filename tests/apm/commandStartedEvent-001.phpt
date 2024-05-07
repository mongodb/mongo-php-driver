--TEST--
MongoDB\Driver\Monitoring\CommandStartedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        var_dump($event->getCommand());
        var_dump($event->getCommandName());
        var_dump($event->getDatabaseName());
        var_dump($event->getOperationId());
        var_dump($event->getRequestId());
        var_dump($event->getServer());

        /* Note: getServerConnectionId() and getServiceId() have more stringent
         * requirements and are tested separately. */
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$manager = create_test_manager();

$subscriber = new MySubscriber();
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand('admin', $command);

?>
--EXPECTF--
object(stdClass)#%d (%d) {
  %A
}
string(4) "ping"
string(5) "admin"
string(%d) "%d"
string(%d) "%d"
object(MongoDB\Driver\Server)#%d (%d) {
  %A
}

--TEST--
MongoDB\Driver\Monitoring\CommandSucceededEvent::getServer()
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
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
        var_dump($event->getServer());
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
Deprecated: %r(Function|Method)%r MongoDB\Driver\Monitoring\CommandSucceededEvent::getServer() is deprecated in %s
object(MongoDB\Driver\Server)#%d (%d) {
  %A
}

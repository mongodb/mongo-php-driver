--TEST--
MongoDB\Driver\Monitoring\addSubscriber(): Adding one subscriber
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
    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event ): void
    {
        echo "- started: ", $event->getCommandName(), "\n";
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event ): void
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event ): void
    {
    }
}

$query = new MongoDB\Driver\Query( [] );
$subscriber = new MySubscriber;

echo "Before addSubscriber\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

echo "After addSubscriber\n";
$cursor = $m->executeQuery( "demo.test", $query );
?>
--EXPECT--
Before addSubscriber
After addSubscriber
- started: find

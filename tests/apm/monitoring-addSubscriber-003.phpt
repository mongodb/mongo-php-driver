--TEST--
MongoDB\Driver\Monitoring\addSubscriber(): Adding one subscriber multiple times
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = new MongoDB\Driver\Manager(URI);

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event )
    {
        echo "- started: ", $event->getCommandName(), "\n";
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event )
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event )
    {
    }
}

$query = new MongoDB\Driver\Query( [] );
$subscriber = new MySubscriber();

echo "Before addSubscriber\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

echo "After addSubscriber\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

echo "After addSubscriber\n";
$cursor = $m->executeQuery( "demo.test", $query );
?>
--EXPECT--
Before addSubscriber
After addSubscriber
- started: find
After addSubscriber
- started: find

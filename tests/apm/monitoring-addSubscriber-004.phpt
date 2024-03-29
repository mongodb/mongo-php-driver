--TEST--
MongoDB\Driver\Monitoring\addSubscriber(): Adding three subscribers
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
    private $instanceName;

    public function __construct( $instanceName )
    {
        $this->instanceName = $instanceName;
    }

    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event ): void
    {
        echo "- ({$this->instanceName}) - started: ", $event->getCommandName(), "\n";
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event ): void
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event ): void
    {
    }
}

$query = new MongoDB\Driver\Query( [] );
$subscriber1 = new MySubscriber( "ONE" );
$subscriber2 = new MySubscriber( "TWO" );
$subscriber3 = new MySubscriber( "THR" );

echo "Before addSubscriber\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber1 );

echo "After addSubscriber (ONE)\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber2 );

echo "After addSubscriber (TWO)\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber3 );

echo "After addSubscriber (THR)\n";
$cursor = $m->executeQuery( "demo.test", $query );
?>
--EXPECT--
Before addSubscriber
After addSubscriber (ONE)
- (ONE) - started: find
After addSubscriber (TWO)
- (ONE) - started: find
- (TWO) - started: find
After addSubscriber (THR)
- (ONE) - started: find
- (TWO) - started: find
- (THR) - started: find

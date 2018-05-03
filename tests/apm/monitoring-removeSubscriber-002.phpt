--TEST--
MongoDB\Driver\Monitoring\removeSubscriber(): Removing one of multiple subscribers
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
    private $instanceName;

    public function __construct( $instanceName )
    {
        $this->instanceName = $instanceName;
    }

    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event )
    {
        echo "- ({$this->instanceName}) - started: ", $event->getCommandName(), "\n";
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event )
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event )
    {
    }
}

$query = new MongoDB\Driver\Query( [] );
$subscriber1 = new MySubscriber( "ONE" );
$subscriber2 = new MySubscriber( "TWO" );

echo "Before addSubscriber\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber1 );

echo "After addSubscriber (ONE)\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\addSubscriber( $subscriber2 );

echo "After addSubscriber (TWO)\n";
$cursor = $m->executeQuery( "demo.test", $query );

MongoDB\Driver\Monitoring\removeSubscriber( $subscriber2 );

echo "After removeSubscriber (TWO)\n";
$cursor = $m->executeQuery( "demo.test", $query );
?>
--EXPECT--
Before addSubscriber
After addSubscriber (ONE)
- (ONE) - started: find
After addSubscriber (TWO)
- (ONE) - started: find
- (TWO) - started: find
After removeSubscriber (TWO)
- (ONE) - started: find

--TEST--
MongoDB\Driver\Monitoring\ServerHeartbeatFailed
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event) {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event) {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event)
    {
        echo "- getAwaited() returns a boolean: ", is_bool($event->getAwaited()) ? 'yes' : 'no', "\n";
        echo "- getDuration() returns an integer: ", is_integer($event->getDuration()) ? 'yes' : 'no', "\n";
        echo "- getHost() returns a string: ", is_string($event->getHost()) ? 'yes' : 'no', "\n";
        echo "- getPort() returns an integer: ", is_integer($event->getPort()) ? 'yes' : 'no', "\n";
    }

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event) {}
    
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event) {}

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event) {}

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event) {}
}

$subscriber = new MySubscriber;
$m->addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$m->executeCommand(DATABASE_NAME, $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
- getAwaited() returns a boolean: yes
- getDuration() returns an integer: yes
- getHost() returns a string: yes
- getPort() returns an integer: yes
===DONE===

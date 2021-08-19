--TEST--
MongoDB\Driver\Monitoring\TopologyClosedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = create_test_manager(URI, [], ['disableClientPersistence' => true]);

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event) {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event) {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event) {}

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event) {}
    
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event) {}

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event)
    {
        echo "- getTopologyId() returns an ObjectId: ", ($event->getTopologyId() instanceof MongoDB\BSON\ObjectId) ? 'yes' : 'no', "\n";
    }

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event) {}
}

$subscriber = new MySubscriber;
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$m->executeCommand(DATABASE_NAME, $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
- getTopologyId() returns an ObjectId: yes
===DONE===

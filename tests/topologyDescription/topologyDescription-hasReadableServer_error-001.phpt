--TEST--
MongoDB\Driver\TopologyDescription::hasReadableServer() (argument with bad type)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class TopologyDescriptionProvider implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $topologyDescription;

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event) {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event) {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event) {}

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event) {}

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event)
    {
        $this->topologyDescription = $event->getNewDescription();
    }

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event) {}

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event) {}

    public function getTopologyDescription()
    {
        $manager = create_test_manager();
        $manager->addSubscriber($this);
        $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
        $manager->removeSubscriber($this);

        return $this->topologyDescription;
    }
}

$tests = [
    null,
    1,
    [],
];

$subscriber = new TopologyDescriptionProvider;
$topologyDescription = $subscriber->getTopologyDescription();

foreach ($tests as $test) {
    throws(function() use ($topologyDescription, $test) {
        $topologyDescription->hasReadableServer($test);
    }, TypeError::class);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got TypeError
OK: Got TypeError
OK: Got TypeError
===DONE===

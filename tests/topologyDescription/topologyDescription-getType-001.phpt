--TEST--
MongoDB\Driver\TopologyDescription::getType()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class TopologyDescriptionProvider implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $topologyDescription;

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event): void {}

    public function serverClosed(MongoDB\Driver\Monitoring\ServerClosedEvent $event): void {}

    public function serverHeartbeatFailed(MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent $event): void {}

    public function serverHeartbeatStarted(MongoDB\Driver\Monitoring\ServerHeartbeatStartedEvent $event): void {}

    public function serverHeartbeatSucceeded(MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent $event): void {}

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event): void {}

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event): void
    {
        $this->topologyDescription = $event->getNewDescription();
    }

    public function topologyClosed(MongoDB\Driver\Monitoring\TopologyClosedEvent $event): void {}

    public function topologyOpening(MongoDB\Driver\Monitoring\TopologyOpeningEvent $event): void {}

    public function getTopologyDescription()
    {
        $manager = create_test_manager();
        $manager->addSubscriber($this);
        $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
        $manager->removeSubscriber($this);

        return $this->topologyDescription;
    }
}

$subscriber = new TopologyDescriptionProvider;
$topologyDescription = $subscriber->getTopologyDescription();
var_dump($topologyDescription->getType());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(%d) "%r(Single|Sharded|ReplicaSetWithPrimary|LoadBalanced)%r"
===DONE===

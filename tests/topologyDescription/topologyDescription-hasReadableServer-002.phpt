--TEST--
MongoDB\Driver\TopologyDescription::hasReadableServer() with ReadPreference argument
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

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event) {}

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event)
    {
        $this->topologyDescription = $event->getNewDescription();
    }

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

$subscriber = new TopologyDescriptionProvider;
$topologyDescription = $subscriber->getTopologyDescription();
$rp = new MongoDB\Driver\ReadPreference('primary');
var_dump($topologyDescription->hasReadableServer($rp));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
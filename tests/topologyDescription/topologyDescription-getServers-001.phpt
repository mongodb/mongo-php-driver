--TEST--
MongoDB\Driver\TopologyDescription::getServers()
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

function isArrayOfServerDescriptions(array $sds) {
    if (count($sds) < 1) {
        return false;
    }

    foreach ($sds as $sd) {
        if (! $sd instanceof MongoDB\Driver\ServerDescription) {
            return false;
        }
    }
    
    return true;
}

$subscriber = new TopologyDescriptionProvider;
$topologyDescription = $subscriber->getTopologyDescription();
$serverDescriptions = $topologyDescription->getServers();

var_dump(isArrayOfServerDescriptions($serverDescriptions));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

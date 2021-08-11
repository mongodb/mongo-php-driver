--TEST--
MongoDB\Driver\Monitoring\TopologyChangedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $topologyDescription;

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event)
    {
        if (isset($this->topologyDescription)) {
            return;
        }

        $this->topologyDescription = $event->getNewDescription();
        echo "- getTopologyId() returns an ObjectId: ", ($event->getTopologyId() instanceof MongoDB\BSON\ObjectId) ? 'yes' : 'no', "\n";
        echo "- getNewDescription() returns a TopologyDescription: ", ($event->getNewDescription() instanceof MongoDB\Driver\TopologyDescription) ? 'yes' : 'no', "\n";
        echo "- getPreviousDescription() returns a TopologyDescription: ", ($event->getPreviousDescription() instanceof MongoDB\Driver\TopologyDescription) ? 'yes' : 'no', "\n";
    }
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
- getNewDescription() returns a TopologyDescription: yes
- getPreviousDescription() returns a TopologyDescription: yes
===DONE===

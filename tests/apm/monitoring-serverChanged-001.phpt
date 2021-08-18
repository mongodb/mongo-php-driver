--TEST--
MongoDB\Driver\Monitoring\ServerChangedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $serverDescription;

    public function serverChanged(MongoDB\Driver\Monitoring\ServerChangedEvent $event)
    {
        if (isset($this->serverDescription)) {
            return;
        }

        $this->serverDescription = $event->getNewDescription();
        echo "- getHost() returns a string: ", is_string($event->getHost()) ? 'yes' : 'no', "\n";
        echo "- getPort() returns an integer: ", is_integer($event->getPort()) ? 'yes' : 'no', "\n";
        echo "- getTopologyId() returns an ObjectId: ", ($event->getTopologyId() instanceof MongoDB\BSON\ObjectId) ? 'yes' : 'no', "\n";
        echo "- getNewDescription() returns a ServerDescription: ", ($event->getNewDescription() instanceof MongoDB\Driver\ServerDescription) ? 'yes' : 'no', "\n";
        echo "- getPreviousDescription() returns a ServerDescription: ", ($event->getPreviousDescription() instanceof MongoDB\Driver\ServerDescription) ? 'yes' : 'no', "\n";
    }

    public function serverOpening(MongoDB\Driver\Monitoring\ServerOpeningEvent $event) {}

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event) {}

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
- getHost() returns a string: yes
- getPort() returns an integer: yes
- getTopologyId() returns an ObjectId: yes
- getNewDescription() returns a ServerDescription: yes
- getPreviousDescription() returns a ServerDescription: yes
===DONE===

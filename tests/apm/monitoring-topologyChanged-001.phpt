--TEST--
MongoDB\Driver\Monitoring\TopologyChangedEvent
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    private $topologyDescription;

    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event)
    {
        if (! $this->topologyDescription) {
            echo "- getTopologyId() returns an object: ", is_object( $event->getTopologyId() ) ? 'yes' : 'no', "\n";
            
            $this->topologyDescription = $event->getNewDescription();
            echo "- topologyDescription->getType() type returns a string: ", is_string( $this->topologyDescription->getType() ) ? 'yes' : 'no', "\n";
            echo "- topologyDescription->getServers() returns an array: ", is_array( $this->topologyDescription->getServers() ) ? 'yes' : 'no', "\n";
        }
    }
}

$subscriber = new MySubscriber;
MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

$command = new MongoDB\Driver\Command(['ping' => 1]);
$m->executeCommand(DATABASE_NAME, $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
- getTopologyId() returns an object: yes
- topologyDescription->getType() type returns a string: yes
- topologyDescription->getServers() returns an array: yes
===DONE===

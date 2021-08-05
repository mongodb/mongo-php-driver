--TEST--
MongoDB\Driver\TopologyDescription::hasWritableServer()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

class MySubscriber implements MongoDB\Driver\Monitoring\SDAMSubscriber
{
    public function topologyChanged(MongoDB\Driver\Monitoring\TopologyChangedEvent $event)
    {
        $expected_types = array(
            MongoDB\Driver\TopologyDescription::TYPE_UNKNOWN,
            MongoDB\Driver\TopologyDescription::TYPE_SINGLE,
            MongoDB\Driver\TopologyDescription::TYPE_SHARDED,
            MongoDB\Driver\TopologyDescription::TYPE_REPLICA_SET_NO_PRIMARY,
            MongoDB\Driver\TopologyDescription::TYPE_REPLICA_SET_WITH_PRIMARY
        );

        $topologyDescription = $event->getNewDescription();
        var_dump($topologyDescription->hasWritableServer());
    }
}

$subscriber = new MySubscriber;
MongoDB\Driver\Monitoring\addSubscriber($subscriber);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(%s)
bool(%s)
bool(%s)
===DONE===

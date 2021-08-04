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
    public function topologyChanged( \MongoDB\Driver\Monitoring\TopologyChangedEvent $event )
    {
        $oid = $event->getTopologyId();

        $topologyDescription = $event->getNewDescription();
        echo "type: ", $topologyDescription->getType(), "\n";
        var_dump($topologyDescription->getServers());
    }
}

$query = new MongoDB\Driver\Query( [] );
$subscriber = new MySubscriber;

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

$cursor = $m->executeQuery( "demo.test", $query );
?>
--EXPECTF--
type: Unknown
array(%d) {%A
}
type: Single
array(%d) {%A
}
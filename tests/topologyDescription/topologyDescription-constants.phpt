--TEST--
MongoDB\Driver\TopologyDescription constants
--FILE--
<?php

var_dump(MongoDB\Driver\TopologyDescription::TYPE_UNKNOWN);
var_dump(MongoDB\Driver\TopologyDescription::TYPE_SINGLE);
var_dump(MongoDB\Driver\TopologyDescription::TYPE_SHARDED);
var_dump(MongoDB\Driver\TopologyDescription::TYPE_REPLICA_SET_NO_PRIMARY);
var_dump(MongoDB\Driver\TopologyDescription::TYPE_REPLICA_SET_WITH_PRIMARY);
var_dump(MongoDB\Driver\TopologyDescription::TYPE_LOAD_BALANCED);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "Unknown"
string(6) "Single"
string(7) "Sharded"
string(19) "ReplicaSetNoPrimary"
string(21) "ReplicaSetWithPrimary"
string(12) "LoadBalanced"
===DONE===

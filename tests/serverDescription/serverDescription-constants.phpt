--TEST--
MongoDB\Driver\ServerDescription constants
--FILE--
<?php

var_dump(MongoDB\Driver\ServerDescription::TYPE_UNKNOWN);
var_dump(MongoDB\Driver\ServerDescription::TYPE_STANDALONE);
var_dump(MongoDB\Driver\ServerDescription::TYPE_MONGOS);
var_dump(MongoDB\Driver\ServerDescription::TYPE_POSSIBLE_PRIMARY);
var_dump(MongoDB\Driver\ServerDescription::TYPE_RS_PRIMARY);
var_dump(MongoDB\Driver\ServerDescription::TYPE_RS_SECONDARY);
var_dump(MongoDB\Driver\ServerDescription::TYPE_RS_ARBITER);
var_dump(MongoDB\Driver\ServerDescription::TYPE_RS_OTHER);
var_dump(MongoDB\Driver\ServerDescription::TYPE_RS_GHOST);
var_dump(MongoDB\Driver\ServerDescription::TYPE_LOAD_BALANCER);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(7) "Unknown"
string(10) "Standalone"
string(6) "Mongos"
string(15) "PossiblePrimary"
string(9) "RSPrimary"
string(11) "RSSecondary"
string(9) "RSArbiter"
string(7) "RSOther"
string(7) "RSGhost"
string(12) "LoadBalancer"
===DONE===

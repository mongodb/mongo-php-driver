--TEST--
MongoDB\Driver\ReadConcern constants
--FILE--
<?php

var_dump(MongoDB\Driver\ReadConcern::LINEARIZABLE);
var_dump(MongoDB\Driver\ReadConcern::LOCAL);
var_dump(MongoDB\Driver\ReadConcern::MAJORITY);
var_dump(MongoDB\Driver\ReadConcern::AVAILABLE);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(12) "linearizable"
string(5) "local"
string(8) "majority"
string(9) "available"
===DONE===

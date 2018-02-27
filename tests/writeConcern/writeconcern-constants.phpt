--TEST--
MongoDB\Driver\WriteConcern constants
--FILE--
<?php

var_dump(MongoDB\Driver\WriteConcern::MAJORITY);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(8) "majority"
===DONE===

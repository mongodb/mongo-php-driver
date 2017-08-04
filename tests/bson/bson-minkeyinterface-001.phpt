--TEST--
MongoDB\BSON\MinKeyInterface is implemented by MongoDB\BSON\MinKey
--FILE--
<?php

$minkey = new MongoDB\BSON\MinKey;
var_dump($minkey instanceof MongoDB\BSON\MinKeyInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

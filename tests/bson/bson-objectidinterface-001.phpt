--TEST--
MongoDB\BSON\ObjectIDInterface is implemented by MongoDB\BSON\ObjectID
--FILE--
<?php

$oid = new MongoDB\BSON\ObjectID;
var_dump($oid instanceof MongoDB\BSON\ObjectIDInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

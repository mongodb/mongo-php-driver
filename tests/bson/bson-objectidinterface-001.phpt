--TEST--
MongoDB\BSON\ObjectIdInterface is implemented by MongoDB\BSON\ObjectId
--FILE--
<?php

$oid = new MongoDB\BSON\ObjectId;
var_dump($oid instanceof MongoDB\BSON\ObjectIdInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

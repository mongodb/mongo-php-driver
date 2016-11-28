--TEST--
MongoDB\BSON\MinKey comparisons
--FILE--
<?php

var_dump(new MongoDB\BSON\MinKey == new MongoDB\BSON\MinKey);
var_dump(new MongoDB\BSON\MinKey < new MongoDB\BSON\MinKey);
var_dump(new MongoDB\BSON\MinKey > new MongoDB\BSON\MinKey);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
bool(false)
===DONE===

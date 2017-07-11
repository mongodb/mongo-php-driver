--TEST--
MongoDB\BSON\MaxKey comparisons
--FILE--
<?php

var_dump(new MongoDB\BSON\MaxKey == new MongoDB\BSON\MaxKey);
var_dump(new MongoDB\BSON\MaxKey < new MongoDB\BSON\MaxKey);
var_dump(new MongoDB\BSON\MaxKey > new MongoDB\BSON\MaxKey);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
===DONE===

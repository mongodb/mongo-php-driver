--TEST--
MongoDB\BSON\Decimal128::jsonSerialize() return value
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
--FILE--
<?php

$decimal = new MongoDB\BSON\Decimal128('12389719287312');
var_dump($decimal->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$numberDecimal"]=>
  string(14) "12389719287312"
}
===DONE===

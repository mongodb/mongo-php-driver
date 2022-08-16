--TEST--
MongoDB\BSON\Decimal128::jsonSerialize() return value
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

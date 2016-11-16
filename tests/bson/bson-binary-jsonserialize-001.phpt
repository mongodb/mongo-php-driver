--TEST--
MongoDB\BSON\Binary::jsonSerialize() return value
--FILE--
<?php

$binary = new MongoDB\BSON\Binary('gargleblaster', 24);
var_dump($binary->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["$binary"]=>
  string(20) "Z2FyZ2xlYmxhc3Rlcg=="
  ["$type"]=>
  string(2) "18"
}
===DONE===

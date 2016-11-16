--TEST--
MongoDB\BSON\Regex::jsonSerialize() return value (with flags)
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('pattern', 'i');
var_dump($regex->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["$regex"]=>
  string(7) "pattern"
  ["$options"]=>
  string(1) "i"
}
===DONE===

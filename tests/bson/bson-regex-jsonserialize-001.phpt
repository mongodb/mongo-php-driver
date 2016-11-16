--TEST--
MongoDB\BSON\Regex::jsonSerialize() return value (without flags)
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('pattern');
var_dump($regex->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["$regex"]=>
  string(7) "pattern"
  ["$options"]=>
  string(0) ""
}
===DONE===

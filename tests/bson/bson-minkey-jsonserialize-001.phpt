--TEST--
MongoDB\BSON\MinKey::jsonSerialize() return value
--FILE--
<?php

$minkey = new MongoDB\BSON\MinKey;
var_dump($minkey->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$minKey"]=>
  int(1)
}
===DONE===

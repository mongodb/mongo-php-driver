--TEST--
MongoDB\BSON\MaxKey::jsonSerialize() return value
--FILE--
<?php

$maxkey = new MongoDB\BSON\MaxKey;
var_dump($maxkey->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$maxKey"]=>
  int(1)
}
===DONE===

--TEST--
MongoDB\BSON\Binary debug output
--FILE--
<?php

$binary = new MongoDB\BSON\Binary('foobar', MongoDB\BSON\Binary::TYPE_GENERIC);

var_dump($binary);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(8) "Zm9vYmFy"
  ["type"]=>
  int(0)
}
===DONE===

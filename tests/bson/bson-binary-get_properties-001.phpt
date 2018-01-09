--TEST--
MongoDB\BSON\Binary get_properties handler (get_object_vars)
--FILE--
<?php

$binary = new MongoDB\BSON\Binary('foobar', MongoDB\BSON\Binary::TYPE_GENERIC);

var_dump(get_object_vars($binary));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["data"]=>
  string(6) "foobar"
  ["type"]=>
  int(0)
}
===DONE===

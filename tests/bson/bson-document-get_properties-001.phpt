--TEST--
MongoDB\BSON\Document get_properties handler (get_object_vars)
--FILE--
<?php

$bson = MongoDB\BSON\Document::fromJSON('{}');

var_dump(get_object_vars($bson));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  ["data"]=>
  string(5) "%s"
  ["length"]=>
  int(5)
}
===DONE===
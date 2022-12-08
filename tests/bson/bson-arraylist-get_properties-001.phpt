--TEST--
MongoDB\BSON\ArrayList get_properties handler (get_object_vars)
--FILE--
<?php

$bson = MongoDB\BSON\ArrayList::fromPHP([]);

var_dump(get_object_vars($bson));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(1) {
  ["data"]=>
  string(8) "BQAAAAA="
}
===DONE===

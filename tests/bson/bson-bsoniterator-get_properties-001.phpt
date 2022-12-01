--TEST--
MongoDB\BSON\BSONIterator get_properties handler (get_object_vars)
--FILE--
<?php

$iterator = MongoDB\BSON\Document::fromJSON('{}')->getIterator();

var_dump(get_object_vars($iterator));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(%d) {
  ["bson"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(5) "%s"
    ["length"]=>
    int(5)
  }
}
===DONE===

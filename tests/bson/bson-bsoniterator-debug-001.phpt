--TEST--
MongoDB\BSON\BSONIterator debug output
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump(MongoDB\BSON\BSONDocument::fromJSON('{ }')->getIterator());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\BSONIterator)#%d (%d) {
  ["bson"]=>
  object(MongoDB\BSON\BSONDocument)#%d (%d) {
    ["data"]=>
    string(5) "%s"
    ["length"]=>
    int(5)
  }
}
===DONE===

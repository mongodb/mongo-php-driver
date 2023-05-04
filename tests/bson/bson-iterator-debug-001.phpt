--TEST--
MongoDB\BSON\Iterator debug output
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump(MongoDB\BSON\Document::fromJSON('{}')->getIterator());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Iterator)#%d (%d) {
  ["bson"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(8) "BQAAAAA="
    ["value"]=>
    object(stdClass)#%d (%d) {
    }
  }
}
===DONE===

--TEST--
MongoDB\BSON\Document can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = MongoDB\BSON\Document::fromJSON('{ "foo": [ 1, 2, 3 ]}');
$clone = clone $bson;

var_dump($clone == $bson);
var_dump($clone === $bson);

unset($bson);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Document)#%d (%d) {
  ["data"]=>
  string(48) "JAAAAARmb28AGgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAAA"
  ["value"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    object(MongoDB\BSON\PackedArray)#%d (%d) {
      ["data"]=>
      string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
      ["value"]=>
      array(3) {
        [0]=>
        int(1)
        [1]=>
        int(2)
        [2]=>
        int(3)
      }
    }
  }
}
===DONE===

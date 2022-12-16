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
}
===DONE===

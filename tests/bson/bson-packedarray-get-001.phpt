--TEST--
MongoDB\BSON\PackedArray::get() index access
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\PackedArray::fromPHP([0, 1, "foo", new MongoDB\BSON\Int64(123)]);
var_dump($document->get(0));
var_dump($document->get(2));
var_dump($document->get(3));

echo throws(function() use ($document) {
    var_dump($document->get(4));
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
int(0)
string(3) "foo"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(3) "123"
}
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find index "4" in BSON data
===DONE===

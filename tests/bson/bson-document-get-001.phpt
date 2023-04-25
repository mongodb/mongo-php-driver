--TEST--
MongoDB\BSON\Document::get() key access
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP(['foo' => 'bar', 'bar' => 'baz']);
var_dump($document->get('foo'));
var_dump($document->get('bar'));

echo throws(function() use ($document) {
    var_dump($document->get('baz'));
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "string"
  ["value"]=>
  string(3) "bar"
}
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(6) "string"
  ["value"]=>
  string(3) "baz"
}
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find key "baz" in BSON data
===DONE===

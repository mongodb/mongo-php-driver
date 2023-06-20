--TEST--
MongoDB\BSON\Document::get() key access
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP([
    'foo' => 'bar',
    'bar' => 'baz',
    'int64' => new MongoDB\BSON\Int64(123),
]);
var_dump($document->get('foo'));
var_dump($document->get('bar'));
var_dump($document->get('int64'));

echo throws(function() use ($document) {
    var_dump($document->get('baz'));
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(3) "bar"
string(3) "baz"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(3) "123"
}
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find key "baz" in BSON data
===DONE===

--TEST--
MongoDB\BSON\Document array access (ArrayAccess methods)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP([
    'foo' => 'bar',
    'bar' => 'baz',
    'int64' => new MongoDB\BSON\Int64(123),
]);

var_dump($document->offsetExists('foo'));
var_dump($document->offsetExists('int64'));
var_dump($document->offsetExists('baz'));
var_dump($document->offsetExists(0));

var_dump($document->offsetGet('foo'));
var_dump($document->offsetGet('int64'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
bool(false)
bool(false)
string(3) "bar"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(3) "123"
}
===DONE===

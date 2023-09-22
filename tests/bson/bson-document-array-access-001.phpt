--TEST--
MongoDB\BSON\Document array access (dimension object accessors)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP([
    'foo' => 'bar',
    'bar' => 'baz',
    'int64' => new MongoDB\BSON\Int64(123),
]);

var_dump(isset($document['foo']));
var_dump(isset($document['int64']));
var_dump(isset($document['baz']));

var_dump($document['foo']);
var_dump($document['int64']);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
bool(false)
string(3) "bar"
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(3) "123"
}
===DONE===

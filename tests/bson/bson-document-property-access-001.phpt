--TEST--
MongoDB\BSON\Document property access
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP([
    'foo' => 'bar',
    'bar' => 'baz',
    'int64' => new MongoDB\BSON\Int64(123),
]);

var_dump(isset($document->foo));
var_dump(isset($document->int64));
var_dump(isset($document->baz));

var_dump($document->foo);
var_dump($document->int64);

echo throws(function() use ($document) {
    var_dump($document->baz);
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

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
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find key "baz" in BSON document
===DONE===

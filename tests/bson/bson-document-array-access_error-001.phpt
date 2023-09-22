--TEST--
MongoDB\BSON\Document array access does not allow writing (dimension object accessors)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$document = MongoDB\BSON\Document::fromPHP([
    'foo' => 'bar',
    'bar' => 'baz',
    'int64' => new MongoDB\BSON\Int64(123),
]);

echo throws(function() use ($document) {
    $document['foo'] = 'baz';
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function() use ($document) {
    unset($document['foo']);
}, MongoDB\Driver\Exception\LogicException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\LogicException
Cannot write to MongoDB\BSON\Document property
OK: Got MongoDB\Driver\Exception\LogicException
Cannot unset MongoDB\BSON\Document property
===DONE===

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
--EXPECT--
string(3) "bar"
string(3) "baz"
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find key "baz" in BSON data
===DONE===

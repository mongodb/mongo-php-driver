--TEST--
MongoDB\BSON\Document::has() checks if key exists
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP(['foo' => 'bar', 'bar' => 'baz']);
var_dump($document->has('foo'));
var_dump($document->has('bar'));
var_dump($document->has('baz'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(false)
===DONE===

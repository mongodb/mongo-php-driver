--TEST--
MongoDB\BSON\BSONDocument::has tests
--FILE--
<?php

$document = MongoDB\BSON\BSONDocument::fromPHP(['foo' => 'bar', 'bar' => 'baz']);
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

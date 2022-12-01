--TEST--
MongoDB\BSON\Document::get tests
--FILE--
<?php

$document = MongoDB\BSON\Document::fromPHP(['foo' => 'bar', 'bar' => 'baz']);
var_dump($document->get('foo'));
var_dump($document->get('bar'));
var_dump($document->get('baz'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(3) "bar"
string(3) "baz"
NULL
===DONE===

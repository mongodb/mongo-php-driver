--TEST--
MongoDB\BSON\Iterator::rewind() tests
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    '{ "foo": "bar", "bar": "foo" }',
];

$iterator = MongoDB\BSON\Document::fromPHP(['foo' => 'bar', 'bar' => 'foo'])->getIterator();

var_dump($iterator->key());
$iterator->rewind();
var_dump($iterator->key());
$iterator->next();
var_dump($iterator->key());
$iterator->rewind();
var_dump($iterator->key());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(3) "foo"
string(3) "foo"
string(3) "bar"
string(3) "foo"
===DONE===

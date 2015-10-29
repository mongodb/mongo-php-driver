--TEST--
BSON\toJSON(): Encoding JSON
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    [],
    [ 'foo' => 'bar' ],
    [ 'foo' => [ 1, 2, 3 ]],
    [ 'foo' => [ 'bar' => 1 ]],
];

foreach ($tests as $value) {
    $bson = fromPHP($value);
    echo toJSON($bson), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
{ }
{ "foo" : "bar" }
{ "foo" : [ 1, 2, 3 ] }
{ "foo" : { "bar" : 1 } }
===DONE===

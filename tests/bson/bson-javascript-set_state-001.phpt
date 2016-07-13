--TEST--
MongoDB\BSON\Javascript::__set_state()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    ['function foo(bar) { return bar; }', null],
    ['function foo(bar) { return bar; }', []],
    ['function foo() { return foo; }', ['foo' => 42]],
    ['function foo() { return id; }', ['id' => new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603')]],
];

foreach ($tests as $test) {
    list($code, $scope) = $test;

    var_export(MongoDB\BSON\Javascript::__set_state([
        'code' => $code,
        'scope' => $scope,
    ]));
    echo "\n\n";
}

// Test with missing scope field
var_export(MongoDB\BSON\Javascript::__set_state([
    'code' => 'function foo(bar) { return bar; }',
]));
echo "\n\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo(bar) { return bar; }',
))

MongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo(bar) { return bar; }',
   'scope' => 
  stdClass::__set_state(array(
  )),
))

MongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo() { return foo; }',
   'scope' => 
  stdClass::__set_state(array(
     'foo' => 42,
  )),
))

MongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo() { return id; }',
   'scope' => 
  stdClass::__set_state(array(
     'id' => 
    MongoDB\BSON\ObjectID::__set_state(array(
       'oid' => '53e2a1c40640fd72175d4603',
    )),
  )),
))

MongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function foo(bar) { return bar; }',
))

===DONE===

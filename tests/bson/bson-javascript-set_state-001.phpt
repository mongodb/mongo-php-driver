--TEST--
MongoDB\BSON\Javascript::__set_state()
--FILE--
<?php

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
%w'code' => 'function foo(bar) { return bar; }',
%w'scope' => NULL,
))

MongoDB\BSON\Javascript::__set_state(array(
%w'code' => 'function foo(bar) { return bar; }',
%w'scope' => 
  %Sarray(
  %S),
))

MongoDB\BSON\Javascript::__set_state(array(
%w'code' => 'function foo() { return foo; }',
%w'scope' => 
  %Sarray(
  %w'foo' => 42,
  %S),
))

MongoDB\BSON\Javascript::__set_state(array(
%w'code' => 'function foo() { return id; }',
%w'scope' => 
  %Sarray(
  %w'id' => 
    MongoDB\BSON\ObjectId::__set_state(array(
    %w'oid' => '53e2a1c40640fd72175d4603',
    )),
  %S),
))

MongoDB\BSON\Javascript::__set_state(array(
%w'code' => 'function foo(bar) { return bar; }',
%w'scope' => NULL,
))

===DONE===

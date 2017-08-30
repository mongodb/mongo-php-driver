--TEST--
MongoDB\BSON\Javascript::getCode()
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

    $js = new MongoDB\BSON\Javascript($code, $scope);
    var_dump($js->getCode());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(33) "function foo(bar) { return bar; }"
string(33) "function foo(bar) { return bar; }"
string(30) "function foo() { return foo; }"
string(29) "function foo() { return id; }"
===DONE===

--TEST--
MongoDB\BSON\Javascript::getCode()
--FILE--
<?php

$tests = [
    ['function(bar) { return bar; }', null],
    ['function(bar) { return bar; }', []],
    ['function() { return foo; }', ['foo' => 42]],
    ['function() { return id; }', ['id' => new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603')]],
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
string(29) "function(bar) { return bar; }"
string(29) "function(bar) { return bar; }"
string(26) "function() { return foo; }"
string(25) "function() { return id; }"
===DONE===

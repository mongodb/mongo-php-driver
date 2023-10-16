--TEST--
MongoDB\BSON\Javascript::__set_state()
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

    var_export(MongoDB\BSON\Javascript::__set_state([
        'code' => $code,
        'scope' => $scope,
    ]));
    echo "\n\n";
}

// Test with missing scope field
var_export(MongoDB\BSON\Javascript::__set_state([
    'code' => 'function(bar) { return bar; }',
]));
echo "\n\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function(bar) { return bar; }',
   'scope' => NULL,
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function(bar) { return bar; }',
   'scope' =>%w
  (object) array(
  ),
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function() { return foo; }',
   'scope' =>%w
  (object) array(
     'foo' => 42,
  ),
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function() { return id; }',
   'scope' =>%w
  (object) array(
     'id' =>%w
    %r\\?%rMongoDB\BSON\ObjectId::__set_state(array(
       'oid' => '53e2a1c40640fd72175d4603',
    )),
  ),
))

%r\\?%rMongoDB\BSON\Javascript::__set_state(array(
   'code' => 'function(bar) { return bar; }',
   'scope' => NULL,
))

===DONE===

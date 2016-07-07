--TEST--
MongoDB\BSON\Javascript serialization
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

    var_dump($js = new MongoDB\BSON\Javascript($code, $scope));
    var_dump($s = serialize($js));
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(33) "function foo(bar) { return bar; }"
}
string(94) "O:23:"MongoDB\BSON\Javascript":1:{s:10:"javascript";s:33:"function foo(bar) { return bar; }";}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(33) "function foo(bar) { return bar; }"
}

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(33) "function foo(bar) { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
  }
}
string(125) "O:23:"MongoDB\BSON\Javascript":2:{s:10:"javascript";s:33:"function foo(bar) { return bar; }";s:5:"scope";O:8:"stdClass":0:{}}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(33) "function foo(bar) { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
  }
}

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(30) "function foo() { return foo; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}
string(137) "O:23:"MongoDB\BSON\Javascript":2:{s:10:"javascript";s:30:"function foo() { return foo; }";s:5:"scope";O:8:"stdClass":1:{s:3:"foo";i:42;}}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(30) "function foo() { return foo; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(29) "function foo() { return id; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["id"]=>
    object(MongoDB\BSON\ObjectID)#%d (%d) {
      ["oid"]=>
      string(24) "53e2a1c40640fd72175d4603"
    }
  }
}
string(205) "O:23:"MongoDB\BSON\Javascript":2:{s:10:"javascript";s:29:"function foo() { return id; }";s:5:"scope";O:8:"stdClass":1:{s:2:"id";O:21:"MongoDB\BSON\ObjectID":1:{s:3:"oid";s:24:"53e2a1c40640fd72175d4603";}}}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(29) "function foo() { return id; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["id"]=>
    object(MongoDB\BSON\ObjectID)#%d (%d) {
      ["oid"]=>
      string(24) "53e2a1c40640fd72175d4603"
    }
  }
}

===DONE===

--TEST--
MongoDB\BSON\Javascript serialization (__serialize and __unserialize)
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
  ["code"]=>
  string(29) "function(bar) { return bar; }"
  ["scope"]=>
  NULL
}
string(97) "O:23:"MongoDB\BSON\Javascript":2:{s:4:"code";s:29:"function(bar) { return bar; }";s:5:"scope";N;}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(29) "function(bar) { return bar; }"
  ["scope"]=>
  NULL
}

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(29) "function(bar) { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
  }
}
string(114) "O:23:"MongoDB\BSON\Javascript":2:{s:4:"code";s:29:"function(bar) { return bar; }";s:5:"scope";O:8:"stdClass":0:{}}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(29) "function(bar) { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
  }
}

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(26) "function() { return foo; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}
string(126) "O:23:"MongoDB\BSON\Javascript":2:{s:4:"code";s:26:"function() { return foo; }";s:5:"scope";O:8:"stdClass":1:{s:3:"foo";i:42;}}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(26) "function() { return foo; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}

object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(25) "function() { return id; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["id"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "53e2a1c40640fd72175d4603"
    }
  }
}
string(194) "O:23:"MongoDB\BSON\Javascript":2:{s:4:"code";s:25:"function() { return id; }";s:5:"scope";O:8:"stdClass":1:{s:2:"id";O:21:"MongoDB\BSON\ObjectId":1:{s:3:"oid";s:24:"53e2a1c40640fd72175d4603";}}}"
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(25) "function() { return id; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["id"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "53e2a1c40640fd72175d4603"
    }
  }
}

===DONE===

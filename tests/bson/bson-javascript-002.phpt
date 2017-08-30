--TEST--
MongoDB\BSON\Javascript debug handler
--FILE--
<?php

$tests = array(
    array(
        'function foo(bar) { return bar; }',
        array(),
    ),
    array(
        'function foo() { return foo; }',
        array('foo' => 42),
    ),
    array(
        'function foo() { return id; }',
        array('id' => new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603')),
    ),
);

foreach ($tests as $test) {
    list($code, $scope) = $test;

    $js = new MongoDB\BSON\Javascript($code, $scope);
    var_dump($js);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(33) "function foo(bar) { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
  }
}
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(30) "function foo() { return foo; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(29) "function foo() { return id; }"
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

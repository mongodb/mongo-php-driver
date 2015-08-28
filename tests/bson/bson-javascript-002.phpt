--TEST--
BSON BSON\Javascript debug handler
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$jsClassname = BSON_NAMESPACE . '\Javascript';
$oidClassname = BSON_NAMESPACE . '\ObjectId';

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
        array('id' => new $oidClassname('53e2a1c40640fd72175d4603')),
    ),
);

foreach ($tests as $test) {
    list($code, $scope) = $test;

    $js = new $jsClassname($code, $scope);
    var_dump($js);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(%SBSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(33) "function foo(bar) { return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
  }
}
object(%SBSON\Javascript)#%d (%d) {
  ["javascript"]=>
  string(30) "function foo() { return foo; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}
object(%SBSON\Javascript)#%d (%d) {
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

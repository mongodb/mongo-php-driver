--TEST--
MongoDB\BSON\Javascript can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$javascript = new MongoDB\BSON\Javascript("function foo(bar) {var baz = bar; var bar = foo; return bar; }", ['foo' => 42]);
$javascript->foo = 'bar';

$clone = clone $javascript;

var_dump($clone == $javascript);
var_dump($clone === $javascript);

unset($javascript);

var_dump($clone);
var_dump($clone->foo);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Javascript)#%d (%d) {
  ["code"]=>
  string(62) "function foo(bar) {var baz = bar; var bar = foo; return bar; }"
  ["scope"]=>
  object(stdClass)#%d (%d) {
    ["foo"]=>
    int(42)
  }
}
string(3) "bar"
===DONE===

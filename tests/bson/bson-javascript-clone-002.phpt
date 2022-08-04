--TEST--
MongoDB\BSON\Javascript can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$javascript = new MongoDB\BSON\Javascript("function foo(bar) {var baz = bar; var bar = foo; return bar; }", ['foo' => 42]);

$clone = clone $javascript;

var_dump($clone == $javascript);
var_dump($clone === $javascript);

unset($javascript);

var_dump($clone);
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
===DONE===

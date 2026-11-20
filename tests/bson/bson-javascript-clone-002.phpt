--TEST--
MongoDB\BSON\Javascript can be cloned without a scope
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$javascript = new MongoDB\BSON\Javascript('function(x) { return x; }');

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
  string(25) "function(x) { return x; }"
  ["scope"]=>
  NULL
}
===DONE===

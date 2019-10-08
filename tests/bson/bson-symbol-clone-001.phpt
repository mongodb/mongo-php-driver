--TEST--
MongoDB\BSON\Symbol can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$test = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "test"} }'));

$symbol = $test->symbol;

$clone = clone $symbol;

var_dump($clone == $symbol);
var_dump($clone === $symbol);

unset($symbol);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Symbol)#%d (1) {
  ["symbol"]=>
  string(4) "test"
}
===DONE===

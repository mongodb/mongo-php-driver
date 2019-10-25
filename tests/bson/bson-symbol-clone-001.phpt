--TEST--
MongoDB\BSON\Symbol can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$test = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "test"} }'));

$symbol = $test->symbol;
$symbol->foo = 'bar';

$clone = clone $symbol;

var_dump($clone == $symbol);
var_dump($clone === $symbol);

unset($symbol);

var_dump($clone);
var_dump($clone->foo);
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
string(3) "bar"
===DONE===

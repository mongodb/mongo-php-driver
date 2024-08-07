--TEST--
MongoDB\BSON\Symbol can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$test = MongoDB\BSON\Document::fromJSON('{ "symbol": {"$symbol": "test"} }')->toPHP();

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

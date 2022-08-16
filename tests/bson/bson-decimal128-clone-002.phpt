--TEST--
MongoDB\BSON\Decimal128 can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

$decimal = new MongoDB\BSON\Decimal128('1234.5678');

$clone = clone $decimal;

var_dump($clone == $decimal);
var_dump($clone === $decimal);

unset($decimal);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Decimal128)#%d (1) {
  ["dec"]=>
  string(9) "1234.5678"
}
===DONE===

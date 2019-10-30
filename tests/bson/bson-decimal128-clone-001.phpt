--TEST--
MongoDB\BSON\Decimal128 can be cloned
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
--FILE--
<?php

$decimal = new MongoDB\BSON\Decimal128('1234.5678');
$decimal->foo = 'bar';

$clone = clone $decimal;

var_dump($clone == $decimal);
var_dump($clone === $decimal);

unset($decimal);

var_dump($clone);
var_dump($clone->foo);
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
string(3) "bar"
===DONE===

--TEST--
MongoDB\BSON\Int64 can be cloned
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$int64 = unserialize('C:18:"MongoDB\BSON\Int64":47:{a:1:{s:7:"integer";s:19:"9223372036854775807";}}');
$int64->foo = 'bar';
$clone = clone $int64;

var_dump($clone == $int64);
var_dump($clone === $int64);

unset($int64);

var_dump($clone);
var_dump($clone->foo);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "9223372036854775807"
}
string(3) "bar"
===DONE===

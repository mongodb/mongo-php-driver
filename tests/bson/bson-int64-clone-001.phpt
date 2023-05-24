--TEST--
MongoDB\BSON\Int64 can be cloned (PHP < 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.2'); ?>
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$int64 = new MongoDB\BSON\Int64('9223372036854775807');
$clone = clone $int64;

var_dump($clone == $int64);
var_dump($clone === $int64);

unset($int64);

var_dump($clone);
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
===DONE===

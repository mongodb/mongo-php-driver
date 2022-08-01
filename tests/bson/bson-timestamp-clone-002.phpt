--TEST--
MongoDB\BSON\Timestamp can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$timestamp = new MongoDB\BSON\Timestamp(1234, 5678);

$clone = clone $timestamp;

var_dump($clone == $timestamp);
var_dump($clone === $timestamp);

unset($timestamp);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Timestamp)#%d (2) {
  ["increment"]=>
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}
===DONE===

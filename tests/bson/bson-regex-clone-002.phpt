--TEST--
MongoDB\BSON\Regex can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$regexp = new MongoDB\BSON\Regex("regexp", "i");

$clone = clone $regexp;

var_dump($clone == $regexp);
var_dump($clone === $regexp);

unset($regexp);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Regex)#%d (2) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
===DONE===

--TEST--
MongoDB\BSON\MaxKey can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$maxKey = new MongoDB\BSON\MaxKey;

$clone = clone $maxKey;

var_dump($clone == $maxKey);
var_dump($clone === $maxKey);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
===DONE===

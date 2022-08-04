--TEST--
MongoDB\BSON\MinKey can be cloned (PHP < 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$minKey = new MongoDB\BSON\MinKey;

$clone = clone $minKey;

var_dump($clone == $minKey);
var_dump($clone === $minKey);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
===DONE===

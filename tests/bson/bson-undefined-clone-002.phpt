--TEST--
MongoDB\BSON\Undefined can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$test = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }'));

$undefined = $test->undefined;

$clone = clone $undefined;

var_dump($clone == $undefined);
var_dump($clone === $undefined);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
===DONE===

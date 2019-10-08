--TEST--
MongoDB\BSON\Undefined can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

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

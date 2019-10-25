--TEST--
MongoDB\BSON\Undefined can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$test = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "undefined": {"$undefined": true} }'));

$undefined = $test->undefined;
$undefined->foo = 'bar';

$clone = clone $undefined;

var_dump($clone == $undefined);
var_dump($clone === $undefined);
var_dump($clone->foo);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
string(3) "bar"
===DONE===

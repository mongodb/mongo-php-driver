--TEST--
MongoDB\BSON\MinKey can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$minKey = new MongoDB\BSON\MinKey;
$minKey->foo = 'bar';

$clone = clone $minKey;

var_dump($clone == $minKey);
var_dump($clone === $minKey);
var_dump($clone->foo);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
string(3) "bar"
===DONE===

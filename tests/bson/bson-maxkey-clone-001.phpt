--TEST--
MongoDB\BSON\MaxKey can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

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

--TEST--
MongoDB\BSON\PackedArray array access (coalesce operator)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$array = MongoDB\BSON\PackedArray::fromPHP([]);

var_dump(isset($array[0]));
var_dump($array[0] ?? 'fallback');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(false)
string(8) "fallback"
===DONE===

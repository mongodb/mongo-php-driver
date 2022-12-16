--TEST--
MongoDB\BSON\PackedArray comparisons
--FILE--
<?php

$foobar = MongoDB\BSON\PackedArray::fromPHP([1, 2, 3]);
$barfoo = MongoDB\BSON\PackedArray::fromPHP([2, 3, 4]);

var_dump($foobar == $barfoo);
var_dump($foobar < $barfoo);
var_dump($foobar > $barfoo);

var_dump($foobar == $foobar);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(true)
bool(false)
bool(true)
===DONE===

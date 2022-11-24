--TEST--
MongoDB\BSON\BSONArray comparisons
--FILE--
<?php

$foobar = MongoDB\BSON\BSONArray::fromPHP([1, 2, 3]);
$barfoo = MongoDB\BSON\BSONArray::fromPHP([2, 3, 4]);

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

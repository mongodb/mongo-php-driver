--TEST--
MongoDB\BSON\Int64 comparisons between objects
--FILE--
<?php

$max = new MongoDB\BSON\Int64('9223372036854775807');
$min = new MongoDB\BSON\Int64('-9223372036854775808');
$zero = new MongoDB\BSON\Int64(0);

var_dump($max == $max);
var_dump($max > $min);
var_dump($max > $zero);

var_dump($min == $min);
var_dump($min < $max);
var_dump($min < $zero);

var_dump($zero == $zero);
var_dump($zero < $max);
var_dump($zero > $min);
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===

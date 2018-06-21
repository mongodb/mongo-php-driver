--TEST--
MongoDB\BSON\Int64 comparisons
--FILE--
<?php

$max = unserialize('C:18:"MongoDB\BSON\Int64":47:{a:1:{s:7:"integer";s:19:"9223372036854775807";}}');
$min = unserialize('C:18:"MongoDB\BSON\Int64":48:{a:1:{s:7:"integer";s:20:"-9223372036854775808";}}');
$zero = unserialize('C:18:"MongoDB\BSON\Int64":28:{a:1:{s:7:"integer";s:1:"0";}}');

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

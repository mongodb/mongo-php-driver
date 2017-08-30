--TEST--
MongoDB\BSON\ObjectId comparisons
--FILE--
<?php

var_dump(new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603') == new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603'));
var_dump(new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603') < new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603'));
var_dump(new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603') > new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603'));

var_dump(new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603') < new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4604'));
var_dump(new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603') > new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4602'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
===DONE===

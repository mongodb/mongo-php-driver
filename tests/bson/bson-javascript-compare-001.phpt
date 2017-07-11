--TEST--
MongoDB\BSON\Javascript comparisons (without scope)
--FILE--
<?php

var_dump(new MongoDB\BSON\Javascript('function() { return 1; }') == new MongoDB\BSON\Javascript('function() { return 1; }'));
var_dump(new MongoDB\BSON\Javascript('function() { return 1; }') < new MongoDB\BSON\Javascript('function() { return 2; }'));
var_dump(new MongoDB\BSON\Javascript('function() { return 1; }') > new MongoDB\BSON\Javascript('function() { return 0; }'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
===DONE===

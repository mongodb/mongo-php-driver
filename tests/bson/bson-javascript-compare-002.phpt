--TEST--
MongoDB\BSON\Javascript comparisons (with scope)
--FILE--
<?php

// Comparison does not consider scope document
var_dump(new MongoDB\BSON\Javascript('function() { return 1; }', ['x' => 1]) == new MongoDB\BSON\Javascript('function() { return 1; }', ['x' => 1]));
var_dump(new MongoDB\BSON\Javascript('function() { return 1; }', ['x' => 1]) == new MongoDB\BSON\Javascript('function() { return 1; }', ['x' => 0]));
var_dump(new MongoDB\BSON\Javascript('function() { return 1; }', ['x' => 1]) == new MongoDB\BSON\Javascript('function() { return 1; }', ['x' => 2]));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
===DONE===

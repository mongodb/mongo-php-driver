--TEST--
MongoDB\BSON\Binary comparisons with null bytes
--FILE--
<?php

var_dump(new MongoDB\BSON\Binary("foo\x00bar", 1) == new MongoDB\BSON\Binary("foo\x00bar", 1));
var_dump(new MongoDB\BSON\Binary("foo\x00bar", 1) < new MongoDB\BSON\Binary("foo\x00bar", 1));
var_dump(new MongoDB\BSON\Binary("foo\x00bar", 1) > new MongoDB\BSON\Binary("foo\x00bar", 1));

// Data length is compared first
var_dump(new MongoDB\BSON\Binary("c\x00", 1) < new MongoDB\BSON\Binary("a\x00a", 0));
var_dump(new MongoDB\BSON\Binary("b\x00b", 0) > new MongoDB\BSON\Binary("a\x00", 1));

// Type is compared second
var_dump(new MongoDB\BSON\Binary("foo\x00bar", 1) < new MongoDB\BSON\Binary("foo\x00bar", 2));
var_dump(new MongoDB\BSON\Binary("foo\x00bar", 1) > new MongoDB\BSON\Binary("foo\x00bar", 0));

// Data is compared last
var_dump(new MongoDB\BSON\Binary("foo\x00bar", 1) < new MongoDB\BSON\Binary("foo\x00bat", 1));
var_dump(new MongoDB\BSON\Binary("foo\x00bar", 1) > new MongoDB\BSON\Binary("foo\x00bap", 1));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
===DONE===

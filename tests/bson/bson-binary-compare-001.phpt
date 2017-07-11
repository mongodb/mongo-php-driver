--TEST--
MongoDB\BSON\Binary comparisons
--FILE--
<?php

var_dump(new MongoDB\BSON\Binary('foobar', 1) == new MongoDB\BSON\Binary('foobar', 1));
var_dump(new MongoDB\BSON\Binary('foobar', 1) < new MongoDB\BSON\Binary('foobar', 1));
var_dump(new MongoDB\BSON\Binary('foobar', 1) > new MongoDB\BSON\Binary('foobar', 1));

// Data length is compared first
var_dump(new MongoDB\BSON\Binary('c', 1) < new MongoDB\BSON\Binary('aa', 0));
var_dump(new MongoDB\BSON\Binary('bb', 0) > new MongoDB\BSON\Binary('a', 1));

// Type is compared second
var_dump(new MongoDB\BSON\Binary('foobar', 1) < new MongoDB\BSON\Binary('foobar', 2));
var_dump(new MongoDB\BSON\Binary('foobar', 1) > new MongoDB\BSON\Binary('foobar', 0));

// Data is compared last
var_dump(new MongoDB\BSON\Binary('foobar', 1) < new MongoDB\BSON\Binary('foobat', 1));
var_dump(new MongoDB\BSON\Binary('foobar', 1) > new MongoDB\BSON\Binary('foobap', 1));

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

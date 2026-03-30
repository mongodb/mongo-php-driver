--TEST--
MongoDB\Driver\WriteConcern equality comparison
--FILE--
<?php

var_dump(new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY) == new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY));
var_dump(new MongoDB\Driver\WriteConcern("foo") == new MongoDB\Driver\WriteConcern("foo"));
var_dump(new MongoDB\Driver\WriteConcern(1, 0, 1) == new MongoDB\Driver\WriteConcern(1, 0, 1));

// False for anything else
var_dump(new MongoDB\Driver\WriteConcern(1) == new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY));
var_dump(new MongoDB\Driver\WriteConcern("foo") == new MongoDB\Driver\WriteConcern("bar"));
var_dump(new MongoDB\Driver\WriteConcern("foo", 0, 1) == new MongoDB\Driver\WriteConcern("foo", 0, 0));
var_dump(new MongoDB\Driver\WriteConcern(1, 0, 1) == new MongoDB\Driver\WriteConcern(1, 0, 0));
var_dump(new MongoDB\Driver\WriteConcern(1, 0) == new MongoDB\Driver\WriteConcern(1, 0, false));

// Object comparison fallback if one value is not a WriteConcern
var_dump(new MongoDB\Driver\WriteConcern(1) == new MongoDB\BSON\Int64('8589934592'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
===DONE===

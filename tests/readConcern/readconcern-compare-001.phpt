--TEST--
MongoDB\Driver\ReadConcern comparisons
--FILE--
<?php

var_dump(new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL) == new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL));
var_dump(new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL) > new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::AVAILABLE));
var_dump(new MongoDB\Driver\ReadConcern(null) < new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::AVAILABLE));
var_dump(new MongoDB\Driver\ReadConcern(null) == new MongoDB\Driver\ReadConcern(null));
var_dump(new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL) < new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::AVAILABLE));

// Object comparison fallback if one value is not a ReadConcern
var_dump(new MongoDB\Driver\ReadConcern() > new MongoDB\BSON\Int64('8589934592'));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
===DONE===

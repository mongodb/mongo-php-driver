--TEST--
MongoDB\BSON\Timestamp comparisons
--FILE--
<?php

var_dump(new MongoDB\BSON\Timestamp(1234, 5678) == new MongoDB\BSON\Timestamp(1234, 5678));
var_dump(new MongoDB\BSON\Timestamp(1234, 5678) < new MongoDB\BSON\Timestamp(1234, 5678));
var_dump(new MongoDB\BSON\Timestamp(1234, 5678) > new MongoDB\BSON\Timestamp(1234, 5678));

// Timestamp is compared first
var_dump(new MongoDB\BSON\Timestamp(1234, 5678) < new MongoDB\BSON\Timestamp(1233, 5679));
var_dump(new MongoDB\BSON\Timestamp(1234, 5678) > new MongoDB\BSON\Timestamp(1235, 5677));

// Increment is compared second
var_dump(new MongoDB\BSON\Timestamp(1234, 5678) < new MongoDB\BSON\Timestamp(1235, 5678));
var_dump(new MongoDB\BSON\Timestamp(1234, 5678) > new MongoDB\BSON\Timestamp(1233, 5678));

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
===DONE===

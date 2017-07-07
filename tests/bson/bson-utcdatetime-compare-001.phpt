--TEST--
MongoDB\BSON\UTCDateTime comparisons
--FILE--
<?php

var_dump(new MongoDB\BSON\UTCDateTime(1234) == new MongoDB\BSON\UTCDateTime(1234));
var_dump(new MongoDB\BSON\UTCDateTime(1234) < new MongoDB\BSON\UTCDateTime(1234));
var_dump(new MongoDB\BSON\UTCDateTime(1234) > new MongoDB\BSON\UTCDateTime(1234));

var_dump(new MongoDB\BSON\UTCDateTime(1234) < new MongoDB\BSON\UTCDateTime(1235));
var_dump(new MongoDB\BSON\UTCDateTime(1234) > new MongoDB\BSON\UTCDateTime(1233));

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

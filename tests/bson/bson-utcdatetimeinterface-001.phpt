--TEST--
MongoDB\BSON\UTCDateTimeInterface is implemented by MongoDB\BSON\UTCDateTime
--FILE--
<?php

$date = new MongoDB\BSON\UTCDateTime('1416445411987');
var_dump($date instanceof MongoDB\BSON\UTCDateTimeInterface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

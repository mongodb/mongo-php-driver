--TEST--
BSON BSON\UTCDateTime::toDateTime() dumping seconds and microseconds
--FILE--
<?php
use MongoDB\BSON as BSON;

$utcdatetime = new BSON\UTCDateTime("1416445411987");
$datetime = $utcdatetime->toDateTime();
echo $datetime->format('U.u'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1416445411.987000
===DONE===

--TEST--
BSON BSON\UTCDateTime::toDateTime() dumping seconds and microseconds
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

$utcdatetime = new BSON\UTCDateTime("1416445411987");
$datetime = $utcdatetime->toDateTime();
echo $datetime->format('U.u'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1416445411.987000
===DONE===

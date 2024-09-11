--TEST--
MongoDB\BSON\UTCDateTime::toDateTime() dumping seconds and microseconds
--INI--
date.timezone=UTC
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime(new MongoDB\BSON\Int64('1416445411987'));
$datetime = $utcdatetime->toDateTime();
var_dump(get_class($datetime));
echo $datetime->format('U.u'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(8) "DateTime"
1416445411.987000
===DONE===

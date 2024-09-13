--TEST--
MongoDB\BSON\UTCDateTime::toDateTimeImmutable() dumping seconds and microseconds
--INI--
date.timezone=UTC
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime(new MongoDB\BSON\Int64('1416445411987'));
$datetime = $utcdatetime->toDateTimeImmutable();
var_dump(get_class($datetime));
echo $datetime->format('U.u'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(17) "DateTimeImmutable"
1416445411.987000
===DONE===

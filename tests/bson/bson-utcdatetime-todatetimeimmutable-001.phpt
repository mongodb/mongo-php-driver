--TEST--
MongoDB\BSON\UTCDateTime::toDateTimeImmutable()
--INI--
date.timezone=America/Los_Angeles
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime(new MongoDB\BSON\Int64('1416445411987'));
$datetime = $utcdatetime->toDateTimeImmutable();
var_dump(get_class($datetime));
var_dump($datetime->format(DATE_RSS));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(17) "DateTimeImmutable"
string(31) "Thu, 20 Nov 2014 01:03:31 +0000"
===DONE===

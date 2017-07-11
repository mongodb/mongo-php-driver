--TEST--
MongoDB\BSON\UTCDateTime::toDateTime()
--INI--
date.timezone=America/Los_Angeles
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime("1416445411987");
$datetime = $utcdatetime->toDateTime();
var_dump($datetime->format(DATE_RSS));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(31) "Thu, 20 Nov 2014 01:03:31 +0000"
===DONE===

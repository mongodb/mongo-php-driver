--TEST--
MongoDB\BSON\UTCDateTime::__toString()
--INI--
date.timezone=America/Los_Angeles
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime("1416445411987");
var_dump((string) $utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(13) "1416445411987"
===DONE===

--TEST--
BSON BSON\UTCDateTime::__toString()
--INI--
date.timezone=America/Los_Angeles
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

$utcdatetime = new BSON\UTCDateTime("1416445411987");
var_dump((string) $utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(13) "1416445411987"
===DONE===

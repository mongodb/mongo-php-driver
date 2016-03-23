--TEST--
BSON BSON\UTCDateTime::toDateTime()
--INI--
date.timezone=America/Los_Angeles
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
use MongoDB\BSON as BSON;

require_once __DIR__ . "/../utils/basic.inc";

$utcdatetime = new BSON\UTCDateTime("1416445411987");
$datetime = $utcdatetime->toDateTime();
var_dump($datetime->format("u"));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(6) "987000"
===DONE===

--TEST--
Test for UTCDateTime and integer parsing
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM implements only an older version of DateTime"); ?>
--INI--
date.timezone=UTC
error_reporting=-1
dislay_errors=1
--FILE--
<?php
echo "As number:\n";
$utcdatetime = new MongoDB\BSON\UTCDateTime(1416445411987);
var_dump($utcdatetime);
var_dump($utcdatetime->toDateTime());

echo "As string:\n";
$utcdatetime = new MongoDB\BSON\UTCDateTime('1416445411987');
var_dump($utcdatetime);
var_dump($utcdatetime->toDateTime());
?>
--EXPECTF--
As number:
object(MongoDB\BSON\UTCDateTime)#%d (1) {
  ["milliseconds"]=>
  %r(string\(13\) "|int\()%r1416445411987%r("|\))%r
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(29) "2014-11-20 01:03:31.987000000"
  ["timezone_type"]=>
  int(1)
  ["timezone"]=>
  string(6) "+00:00"
}
As string:
object(MongoDB\BSON\UTCDateTime)#%d (1) {
  ["milliseconds"]=>
  %r(string\(13\) "|int\()%r1416445411987%r("|\))%r
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(29) "2014-11-20 01:03:31.987000000"
  ["timezone_type"]=>
  int(1)
  ["timezone"]=>
  string(6) "+00:00"
}

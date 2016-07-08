--TEST--
BSON BSON\UTCDateTime serialization
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    '0',
    '-1416445411987',
    '1416445411987',
];

foreach ($tests as $milliseconds) {
    var_dump($udt = new MongoDB\BSON\UTCDateTime($milliseconds));
    var_dump($s = serialize($udt));
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(1\) "|%r0%r"|\)%r
}
string(60) "O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";%ri:|s:1:"%r0%r"?%r;}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(1\) "|%r0%r"|\)%r
}

object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(14\) "|%r-1416445411987%r"|\)%r
}
string(73) "O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";%ri:|s:14:"%r-1416445411987%r"?%r;}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(14\) "|%r-1416445411987%r"|\)%r
}

object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(13\) "|%r1416445411987%r"|\)%r
}
string(72) "O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";%ri:|s:13:"%r1416445411987%r"?%r;}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(13\) "|%r1416445411987%r"|\)%r
}

===DONE===

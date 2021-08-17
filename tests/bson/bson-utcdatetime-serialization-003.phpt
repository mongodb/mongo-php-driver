--TEST--
MongoDB\BSON\UTCDateTime serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

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
  string(1) "0"
}
string(64) "O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:1:"0";}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(1) "0"
}

object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(14) "-1416445411987"
}
string(78) "O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:14:"-1416445411987";}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(14) "-1416445411987"
}

object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
string(77) "O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:13:"1416445411987";}"
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}

===DONE===

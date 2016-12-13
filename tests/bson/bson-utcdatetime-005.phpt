--TEST--
MongoDB\BSON\UTCDateTime construction from DateTime
--INI--
date.timezone=UTC
--FILE--
<?php

$tests = [
    new DateTime(),
    new DateTime('@1215282385'),
    new DateTime('2011-01-01T15:03:01.012345Z'),
    new DateTime('2050-11-12T13:14:15.999999Z'),
];

foreach ($tests as $test) {
    var_dump(new MongoDB\BSON\UTCDateTime($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "%d"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1215282385000"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1293894181012"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "2551871655999"
}
===DONE===

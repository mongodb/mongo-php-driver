--TEST--
BSON BSON\UTCDateTime construction from DateTimeImmutable
--SKIPIF--
<?php if (!version_compare(phpversion(), "5.5", '>=')) echo "skip >= PHP 5.5 needed\n"; ?>
--FILE--
<?php

$tests = [
    new DateTimeImmutable(),
    new DateTimeImmutable('@1215282385'),
    new DateTimeImmutable('2011-01-01T15:03:01.012345Z'),
    new DateTimeImmutable('2050-11-12T13:14:15.999999Z'),
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
  int(%d000)
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(1215282385000)
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(1293894181012)
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(2551871655999)
}
===DONE===

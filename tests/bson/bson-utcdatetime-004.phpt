--TEST--
BSON BSON\UTCDateTime constructor defaults to current time
--FILE--
<?php

$tests = [
    new MongoDB\BSON\UTCDateTime,
    new MongoDB\BSON\UTCDateTime(null),
];

foreach ($tests as $test) {
    var_dump($test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(%SBSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(%d)
}
object(%SBSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  int(%d)
}
===DONE===

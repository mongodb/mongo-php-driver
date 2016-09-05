--TEST--
MongoDB\BSON\UTCDateTime constructor defaults to current time
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
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "%d"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "%d"
}
===DONE===

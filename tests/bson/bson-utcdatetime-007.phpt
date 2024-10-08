--TEST--
MongoDB\BSON\UTCDateTime constructor truncates floating point values
--FILE--
<?php

$tests = [
    new MongoDB\BSON\UTCDateTime(1416445411987.0),
    new MongoDB\BSON\UTCDateTime(2147483647.0),
    new MongoDB\BSON\UTCDateTime(1234.5678),
];

foreach ($tests as $test) {
    var_dump($test);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\BSON\UTCDateTime::__construct(): Creating a MongoDB\BSON\UTCDateTime instance with a float is deprecated and will be removed in ext-mongodb 2.0 in %s on line %d

Deprecated: MongoDB\BSON\UTCDateTime::__construct(): Creating a MongoDB\BSON\UTCDateTime instance with a float is deprecated and will be removed in ext-mongodb 2.0 in %s on line %d

Deprecated: MongoDB\BSON\UTCDateTime::__construct(): Creating a MongoDB\BSON\UTCDateTime instance with a float is deprecated and will be removed in ext-mongodb 2.0 in %s on line %d
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(10) "2147483647"
}
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(4) "1234"
}
===DONE===

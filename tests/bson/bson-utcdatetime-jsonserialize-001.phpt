--TEST--
MongoDB\BSON\UTCDateTime::jsonSerialize() return value
--FILE--
<?php

$udt = new MongoDB\BSON\UTCDateTime(new DateTime('2016-10-11 13:34:26.817 UTC'));
var_dump($udt->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$date"]=>
  array(1) {
    ["$numberLong"]=>
    string(13) "1476192866817"
  }
}
===DONE===

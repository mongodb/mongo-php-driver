--TEST--
MongoDB\BSON\UTCDateTime construction from 64-bit integer as string
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$utcdatetime = new MongoDB\BSON\UTCDateTime('1416445411987');

var_dump($utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\BSON\UTCDateTime::__construct(): Creating a MongoDB\BSON\UTCDateTime instance with a string is deprecated and will be removed in ext-mongodb 2.0 in %s
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
===DONE===

--TEST--
MongoDB\BSON\UTCDateTime construction from Int64 object
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$utcdatetime = new MongoDB\BSON\UTCDateTime(new MongoDB\BSON\Int64('1416445411987'));

var_dump($utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
===DONE===

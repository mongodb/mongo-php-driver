--TEST--
MongoDB\BSON\UTCDateTime debug handler
--FILE--
<?php

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

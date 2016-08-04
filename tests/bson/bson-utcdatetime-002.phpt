--TEST--
MongoDB\BSON\UTCDateTime debug handler
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime('1416445411987');

var_dump($utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(13\) "|%r1416445411987%r"|\)%r
}
===DONE===

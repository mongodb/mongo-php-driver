--TEST--
MongoDB\BSON\UTCDateTime debug handler
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime(2147483647);

var_dump($utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  %rint\(|string\(10\) "|%r2147483647%r"|\)%r
}
===DONE===

--TEST--
MongoDB\BSON\UTCDateTime get_properties handler (get_object_vars)
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime('1416445411987');

var_dump(get_object_vars($utcdatetime));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
===DONE===

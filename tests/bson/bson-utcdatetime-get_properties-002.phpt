--TEST--
MongoDB\BSON\UTCDateTime get_properties handler (foreach)
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime('1416445411987');

foreach ($utcdatetime as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(12) "milliseconds"
string(13) "1416445411987"
===DONE===

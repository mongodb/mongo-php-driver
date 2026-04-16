--TEST--
MongoDB\BSON\UTCDateTime get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime(1000);

var_dump($utcdatetime);

print_r($utcdatetime);

var_dump((array) $utcdatetime);

var_export($utcdatetime);
echo "\n";

$restored = eval('return ' . var_export($utcdatetime, true) . ';');
var_dump($utcdatetime == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (1) {
  ["milliseconds"]=>
  string(4) "1000"
}
MongoDB\BSON\UTCDateTime Object
(
    [milliseconds] => 1000
)
array(1) {
  ["milliseconds"]=>
  string(4) "1000"
}
%r\\?%rMongoDB\BSON\UTCDateTime::__set_state(array(
   'milliseconds' => '1000',
))
bool(true)
===DONE===

--TEST--
MongoDB\BSON\Timestamp get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$timestamp = new MongoDB\BSON\Timestamp(1, 2);

var_dump($timestamp);

print_r($timestamp);

var_dump((array) $timestamp);

var_export($timestamp);
echo "\n";

$restored = eval('return ' . var_export($timestamp, true) . ';');
var_dump($timestamp == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Timestamp)#%d (2) {
  ["increment"]=>
  string(1) "1"
  ["timestamp"]=>
  string(1) "2"
}
MongoDB\BSON\Timestamp Object
(
    [increment] => 1
    [timestamp] => 2
)
array(2) {
  ["increment"]=>
  string(1) "1"
  ["timestamp"]=>
  string(1) "2"
}
%r\\?%rMongoDB\BSON\Timestamp::__set_state(array(
   'increment' => '1',
   'timestamp' => '2',
))
bool(true)
===DONE===

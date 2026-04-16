--TEST--
MongoDB\BSON\Int64 get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$int64 = new MongoDB\BSON\Int64(42);

var_dump($int64);

print_r($int64);

var_dump((array) $int64);

var_export($int64);
echo "\n";

$restored = eval('return ' . var_export($int64, true) . ';');
var_dump($int64 == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (1) {
  ["integer"]=>
  string(2) "42"
}
MongoDB\BSON\Int64 Object
(
    [integer] => 42
)
array(1) {
  ["integer"]=>
  string(2) "42"
}
%r\\?%rMongoDB\BSON\Int64::__set_state(array(
   'integer' => '42',
))
bool(true)
===DONE===

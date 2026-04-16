--TEST--
MongoDB\BSON\PackedArray get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$packedarray = MongoDB\BSON\PackedArray::fromPHP([1, 'foo']);

var_dump($packedarray);

print_r($packedarray);

var_dump((array) $packedarray);

var_export($packedarray);
echo "\n";

$restored = eval('return ' . var_export($packedarray, true) . ';');
var_dump($packedarray == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\PackedArray)#%d (2) {
  ["data"]=>
  string(32) "FwAAABAwAAEAAAACMQAEAAAAZm9vAAA="
  ["value"]=>
  array(2) {
    [0]=>
    int(1)
    [1]=>
    string(3) "foo"
  }
}
MongoDB\BSON\PackedArray Object
(
    [data] => FwAAABAwAAEAAAACMQAEAAAAZm9vAAA=
    [value] => Array
        (
            [0] => 1
            [1] => foo
        )

)
array(1) {
  ["data"]=>
  string(32) "FwAAABAwAAEAAAACMQAEAAAAZm9vAAA="
}
%r\\?%rMongoDB\BSON\PackedArray::__set_state(array(
   'data' => 'FwAAABAwAAEAAAACMQAEAAAAZm9vAAA=',
))
bool(true)
===DONE===

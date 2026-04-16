--TEST--
MongoDB\BSON\Binary get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$binary = new MongoDB\BSON\Binary('foobar', MongoDB\BSON\Binary::TYPE_GENERIC);

var_dump($binary);

print_r($binary);

var_dump((array) $binary);

var_export($binary);
echo "\n";

$restored = eval('return ' . var_export($binary, true) . ';');
var_dump($binary == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Binary)#%d (2) {
  ["data"]=>
  string(8) "Zm9vYmFy"
  ["type"]=>
  int(0)
}
MongoDB\BSON\Binary Object
(
    [data] => Zm9vYmFy
    [type] => 0
)
array(2) {
  ["data"]=>
  string(6) "foobar"
  ["type"]=>
  int(0)
}
%r\\?%rMongoDB\BSON\Binary::__set_state(array(
   'data' => 'foobar',
   'type' => 0,
))
bool(true)
===DONE===

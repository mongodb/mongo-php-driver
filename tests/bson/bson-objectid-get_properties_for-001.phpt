--TEST--
MongoDB\BSON\ObjectId get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$oid = new MongoDB\BSON\ObjectId('53e2a1c40640fd72175d4603');

var_dump($oid);

print_r($oid);

var_dump((array) $oid);

var_export($oid);
echo "\n";

$restored = eval('return ' . var_export($oid, true) . ';');
var_dump($oid == $restored);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectId)#%d (1) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
MongoDB\BSON\ObjectId Object
(
    [oid] => 53e2a1c40640fd72175d4603
)
array(1) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
%r\\?%rMongoDB\BSON\ObjectId::__set_state(array(
   'oid' => '53e2a1c40640fd72175d4603',
))
bool(true)
===DONE===

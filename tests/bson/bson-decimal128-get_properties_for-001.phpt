--TEST--
MongoDB\BSON\Decimal128 get_properties_for handler (var_dump, print_r, array cast, var_export)
--FILE--
<?php

$decimal = new MongoDB\BSON\Decimal128('1234.5678');

var_dump($decimal);

print_r($decimal);

var_dump((array) $decimal);

var_export($decimal);
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Decimal128)#%d (1) {
  ["dec"]=>
  string(9) "1234.5678"
}
MongoDB\BSON\Decimal128 Object
(
    [dec] => 1234.5678
)
array(1) {
  ["dec"]=>
  string(9) "1234.5678"
}
%r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
   'dec' => '1234.5678',
))
===DONE===

--TEST--
MongoDB\BSON\Decimal128 debug handler
--FILE--
<?php

$tests = [
    1234.5678,
    NAN,
    INF,
];

foreach ($tests as $test) {
    var_dump(new MongoDB\BSON\Decimal128($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(9) "1234.5678"
}
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(3) "NaN"
}
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(8) "Infinity"
}
===DONE===

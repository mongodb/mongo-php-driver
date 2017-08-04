--TEST--
MongoDB\BSON\Decimal128Interface is implemented by MongoDB\BSON\Decimal128
--FILE--
<?php

$decimal = new MongoDB\BSON\Decimal128('1234.5678');
var_dump($decimal instanceof MongoDB\BSON\Decimal128Interface);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

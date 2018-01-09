--TEST--
MongoDB\BSON\Decimal128 get_properties handler (foreach)
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
--FILE--
<?php

$decimal = new MongoDB\BSON\Decimal128('1234.5678');

foreach ($decimal as $key => $value) {
    var_dump($key);
    var_dump($value);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(3) "dec"
string(9) "1234.5678"
===DONE===

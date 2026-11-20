--TEST--
MongoDB\BSON\Int64 operation errors: shift by negative number
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$value = new MongoDB\BSON\Int64(10);

echo throws(function() use ($value) {
    var_dump($value << -1);
}, ArithmeticError::class), "\n";

echo throws(function() use ($value) {
    var_dump($value >> -1);
}, ArithmeticError::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got ArithmeticError
Bit shift by negative number
OK: Got ArithmeticError
Bit shift by negative number
===DONE===

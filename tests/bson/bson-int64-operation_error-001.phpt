--TEST--
MongoDB\BSON\Int64 operation errors
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$value = new MongoDB\BSON\Int64(10);

// Unsupported type
echo throws(function() use ($value) {
    var_dump($value + 'foo');
}, TypeError::class), "\n";

// Division by zero
echo throws(function() use ($value) {
    var_dump($value / 0);
}, DivisionByZeroError::class), "\n";
echo throws(function() use ($value) {
    var_dump($value % 0);
}, DivisionByZeroError::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got TypeError
Unsupported operand types: MongoDB\BSON\Int64 + string
OK: Got DivisionByZeroError
Division by zero
OK: Got DivisionByZeroError
Division by zero
===DONE===

--TEST--
MongoDB\BSON\Int64 operation errors (PHP < 8>)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '8.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$value = new MongoDB\BSON\Int64(10);

// Unsupported type. This yields a warning about a non-numeric value
echo raises(function() use ($value) {
    var_dump($value + 'foo');
}, E_WARNING), "\n";

// Division by zero
echo throws(function() use ($value) {
    var_dump($value / 0);
}, DivisionByZeroError::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got E_WARNING
A non-numeric value encountered
OK: Got DivisionByZeroError
Division by zero
===DONE===

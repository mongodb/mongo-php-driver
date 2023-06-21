--TEST--
MongoDB\BSON\Int64 comparisons with scalars
--FILE--
<?php

$int64 = new MongoDB\BSON\Int64('123');

$tests = [
    'matching int' => 123,
    'matching numeric string' => '123',
    'matching float' => 123.0,
    'matching float string' => '123.0',

    'wrong int' => 456,
    'wrong numeric string' => '456',
    'wrong float' => 456.0,
    'wrong float string' => '456.0',
    'float with decimals' => 123.456,
    'string float with decimals' => '123.456',

    'non-numeric string' => 'foo',
];

foreach ($tests as $name => $value) {
    printf('Testing %s: %s' . PHP_EOL, $name, var_export($int64 == $value, true));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing matching int: true
Testing matching numeric string: true
Testing matching float: true
Testing matching float string: true
Testing wrong int: false
Testing wrong numeric string: false
Testing wrong float: false
Testing wrong float string: false
Testing float with decimals: false
Testing string float with decimals: false
Testing non-numeric string: false
===DONE===

--TEST--
MongoDB\BSON\Int64 comparisons with scalars (64-bit values)
--FILE--
<?php

// Use 2**33 to ensure it still fits in a float
$int64 = new MongoDB\BSON\Int64('8589934592');

$tests = [
    'matching numeric string' => '8589934592',
    'matching float' => 8589934592.0,
    'matching float string' => '8589934592.0',

    'wrong numeric string' => '8589934593',
    'wrong float' => 8589934593.0,
    'wrong float string' => '8589934593.0',
    'float with decimals' => 8589934592.1,
    'string float with decimals' => '8589934592.1',
];

foreach ($tests as $name => $value) {
    printf('Testing %s: %s' . PHP_EOL, $name, var_export($int64 == $value, true));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing matching numeric string: true
Testing matching float: true
Testing matching float string: true
Testing wrong numeric string: false
Testing wrong float: false
Testing wrong float string: false
Testing float with decimals: false
Testing string float with decimals: false
===DONE===

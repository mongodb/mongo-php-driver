--TEST--
MongoDB\BSON\Int64 comparisons with scalars (64-bit values, 64-bit platforms only)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

// Use 2**33 to ensure it still fits in a float
$int64 = new MongoDB\BSON\Int64('8589934592');

$tests = [
    'matching int' => 8589934592,
    'wrong int' => 8589934593,
];

foreach ($tests as $name => $value) {
    printf('Testing %s: %s' . PHP_EOL, $name, var_export($int64 == $value, true));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing matching int: true
Testing wrong int: false
===DONE===

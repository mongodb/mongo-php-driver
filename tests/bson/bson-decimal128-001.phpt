--TEST--
MongoDB\BSON\Decimal128
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
--FILE--
<?php

$tests = [
    '1234.5678',
    '-1234.5678',
    '1234e5',
    '+1234E+5',
    '1234.56e-78',
    '-234.567',
    '2345E+6',
    '+2345E-6',
    1234.5678,
    -1234.5678,
    -234.567,
    1234e5,
    1234.56e-78,
];

foreach ($tests as $test) {
    printf("%s\n", new MongoDB\BSON\Decimal128($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1234.5678
-1234.5678
1.234E+8
1.234E+8
1.23456E-75
-234.567
2.345E+9
0.002345
1234.5678
-1234.5678
-234.567
123400000
1.23456E-75
===DONE===

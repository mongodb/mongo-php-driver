--TEST--
MongoDB\BSON\Decimal128 NaN values
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
--FILE--
<?php

$tests = [
    acos(8),
    NAN,
    'nan',
    'Nan',
    'NaN',
    'NAN',
];

foreach ($tests as $test) {
    printf("%s\n", new MongoDB\BSON\Decimal128($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NaN
NaN
NaN
NaN
NaN
NaN
===DONE===

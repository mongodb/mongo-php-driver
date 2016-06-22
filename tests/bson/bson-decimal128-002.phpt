--TEST--
BSON\Decimal128 NaN values
--SKIPIF--
<?php require __DIR__ . '/../utils/basic-skipif.inc'?>
--FILE--
<?php
require_once __DIR__ . '/../utils/basic.inc';

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

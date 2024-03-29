--TEST--
MongoDB\BSON\Decimal128 Infinity values
--FILE--
<?php

$tests = [
    1.7976931348623158e+308+1.7976931348623158e+308,
    INF,
    'inf',
    'Inf',
    'INF',
    'infinity',
    'Infinity',
    'INFINITY',
];

foreach ($tests as $test) {
    printf("%s\n", new MongoDB\BSON\Decimal128($test));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Infinity
Infinity
Infinity
Infinity
Infinity
Infinity
Infinity
Infinity
===DONE===

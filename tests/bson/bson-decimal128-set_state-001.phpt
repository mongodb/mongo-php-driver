--TEST--
MongoDB\BSON\Decimal128::__set_state()
--FILE--
<?php

$tests = [
    '1234.5678',
    '-1234.5678',
    'Infinity',
    'INF',
    'NaN',
    'NAN',
];

foreach ($tests as $value) {
    var_export(MongoDB\BSON\Decimal128::__set_state([
        'dec' => $value,
    ]));
    echo "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
%w'dec' => '1234.5678',
))

%r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
%w'dec' => '-1234.5678',
))

%r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
%w'dec' => 'Infinity',
))

%r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
%w'dec' => 'Infinity',
))

%r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
%w'dec' => 'NaN',
))

%r\\?%rMongoDB\BSON\Decimal128::__set_state(array(
%w'dec' => 'NaN',
))

===DONE===

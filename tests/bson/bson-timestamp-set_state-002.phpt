--TEST--
MongoDB\BSON\Timestamp::__set_state() (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    [4294967295, 0],
    [0, 4294967295],
];

foreach ($tests as $test) {
    list($increment, $timestamp) = $test;

    var_export(MongoDB\BSON\Timestamp::__set_state([
        'increment' => $increment,
        'timestamp' => $timestamp,
    ]));
    echo "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\Timestamp::__set_state(array(
%w'increment' => '4294967295',
%w'timestamp' => '0',
))

MongoDB\BSON\Timestamp::__set_state(array(
%w'increment' => '0',
%w'timestamp' => '4294967295',
))

===DONE===

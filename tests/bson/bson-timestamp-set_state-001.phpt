--TEST--
MongoDB\BSON\Timestamp::__set_state()
--FILE--
<?php

$tests = [
    [1234, 5678],
    [2147483647, 0],
    [0, 2147483647],
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
%w'increment' => '1234',
%w'timestamp' => '5678',
))

MongoDB\BSON\Timestamp::__set_state(array(
%w'increment' => '2147483647',
%w'timestamp' => '0',
))

MongoDB\BSON\Timestamp::__set_state(array(
%w'increment' => '0',
%w'timestamp' => '2147483647',
))

===DONE===

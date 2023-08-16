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
%r\\?%rMongoDB\BSON\Timestamp::__set_state(array(
   'increment' => '1234',
   'timestamp' => '5678',
))

%r\\?%rMongoDB\BSON\Timestamp::__set_state(array(
   'increment' => '2147483647',
   'timestamp' => '0',
))

%r\\?%rMongoDB\BSON\Timestamp::__set_state(array(
   'increment' => '0',
   'timestamp' => '2147483647',
))

===DONE===

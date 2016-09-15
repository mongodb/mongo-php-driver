--TEST--
MongoDB\BSON\UTCDateTime::__set_state() (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    0,
    -1416445411987,
    1416445411987,
];

foreach ($tests as $milliseconds) {
    var_export(MongoDB\BSON\UTCDateTime::__set_state([
        'milliseconds' => $milliseconds,
    ]));
    echo "\n\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\BSON\UTCDateTime::__set_state(array(
%w'milliseconds' => '0',
))

MongoDB\BSON\UTCDateTime::__set_state(array(
%w'milliseconds' => '-1416445411987',
))

MongoDB\BSON\UTCDateTime::__set_state(array(
%w'milliseconds' => '1416445411987',
))

===DONE===

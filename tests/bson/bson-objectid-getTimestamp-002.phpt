--TEST--
MongoDB\BSON\ObjectId::getTimestamp: Ensure that the Timestamp field is represented as an unsigned 32-bit integer
--FILE--
<?php
date_default_timezone_set("UTC");

function create_object_id($oid_string)
{
    $objectid = new MongoDB\BSON\ObjectId($oid_string);
    $ts = sprintf("%u", $objectid->getTimestamp());
    echo $ts, "\n";
    echo date_create("@{$ts}")->format("Y-m-d H:i:s"), "\n";
}

create_object_id('000000000000000000000000');
create_object_id('7FFFFFFF0000000000000000');
create_object_id('800000000000000000000000');
create_object_id('FFFFFFFF0000000000000000');

?>
--EXPECT--
0
1970-01-01 00:00:00
2147483647
2038-01-19 03:14:07
2147483648
2038-01-19 03:14:08
4294967295
2106-02-07 06:28:15
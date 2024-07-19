--TEST--
MongoDB\BSON\UTCDateTime constructor requires strings to parse as 64-bit integers
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\BSON\UTCDateTime('1234.5678');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

// Out-of-range values
echo throws(function() {
    new MongoDB\BSON\UTCDateTime('9223372036854775808');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\UTCDateTime('-9223372036854775809');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\UTCDateTime('18446744073709551615');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";


?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "1234.5678" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "9223372036854775808" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "-9223372036854775809" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "18446744073709551615" as 64-bit integer for MongoDB\BSON\UTCDateTime initialization
===DONE===

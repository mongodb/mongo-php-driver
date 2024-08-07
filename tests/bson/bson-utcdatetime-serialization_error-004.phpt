--TEST--
MongoDB\BSON\UTCDateTime unserialization requires "milliseconds" string to parse as 64-bit integer (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:9:"1234.5678";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

// Out-of-range values
echo throws(function() {
    unserialize('O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:19:"9223372036854775808";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:20:"-9223372036854775809";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('O:24:"MongoDB\BSON\UTCDateTime":1:{s:12:"milliseconds";s:20:"18446744073709551615";}');
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

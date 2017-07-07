--TEST--
MongoDB\BSON\Binary constructor requires 16-byte data length for UUID types
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\Binary('0123456789abcde', MongoDB\BSON\Binary::TYPE_OLD_UUID);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\Binary('0123456789abcdefg', MongoDB\BSON\Binary::TYPE_OLD_UUID);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\Binary('0123456789abcde', MongoDB\BSON\Binary::TYPE_UUID);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\Binary('0123456789abcdefg', MongoDB\BSON\Binary::TYPE_UUID);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 15 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 17 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 15 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 17 given
===DONE===

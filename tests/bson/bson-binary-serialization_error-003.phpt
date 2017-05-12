--TEST--
MongoDB\BSON\Binary unserialization requires 16-byte data length for UUID types
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":55:{a:2:{s:4:"data";s:15:"0123456789abcde";s:4:"type";i:3;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":57:{a:2:{s:4:"data";s:17:"0123456789abcdefg";s:4:"type";i:3;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":55:{a:2:{s:4:"data";s:15:"0123456789abcde";s:4:"type";i:4;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":57:{a:2:{s:4:"data";s:17:"0123456789abcdefg";s:4:"type";i:4;}}');
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

--TEST--
MongoDB\BSON\Binary unserialization requires unsigned 8-bit integer for type
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":46:{a:2:{s:4:"data";s:6:"foobar";s:4:"type";i:-1;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":47:{a:2:{s:4:"data";s:6:"foobar";s:4:"type";i:256;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected type to be an unsigned 8-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected type to be an unsigned 8-bit integer, 256 given
===DONE===

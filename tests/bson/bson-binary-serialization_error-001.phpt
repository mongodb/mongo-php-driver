--TEST--
MongoDB\BSON\Binary unserialization requires "data" string and "type" integer fields
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":30:{a:1:{s:4:"data";s:6:"foobar";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":21:{a:1:{s:4:"type";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Binary":45:{a:2:{s:4:"data";i:0;s:4:"type";s:6:"foobar";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary initialization requires "data" string and "type" integer fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary initialization requires "data" string and "type" integer fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary initialization requires "data" string and "type" integer fields
===DONE===

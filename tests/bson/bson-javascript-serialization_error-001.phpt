--TEST--
MongoDB\BSON\Javascript unserialization requires "code" string field
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:23:"MongoDB\BSON\Javascript":21:{a:1:{s:4:"code";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Javascript initialization requires "code" string field
===DONE===

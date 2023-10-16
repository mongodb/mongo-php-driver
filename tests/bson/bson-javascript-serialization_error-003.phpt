--TEST--
MongoDB\BSON\Javascript unserialization does not allow code to contain null bytes (Serializable interface)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('C:23:"MongoDB\BSON\Javascript":51:{a:1:{s:4:"code";s:26:"function() { return ' . "'\0'" . '; }";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Code cannot contain null bytes
===DONE===

--TEST--
MongoDB\BSON\Symbol unserialization does not allow string to contain null bytes (__serialize and __unserialize)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    unserialize('O:19:"MongoDB\BSON\Symbol":1:{s:6:"symbol";s:5:"test' . "\0" . '";}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Symbol cannot contain null bytes
===DONE===

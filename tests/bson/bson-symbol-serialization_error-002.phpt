--TEST--
MongoDB\BSON\Symbol unserialization does not allow code to contain null bytes
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:19:"MongoDB\BSON\Symbol":57:{a:1:{s:6:"symbol";s:30:"function foo() { return ' . "'\0'" . '; }";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Symbol cannot contain null bytes
===DONE===

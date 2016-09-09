--TEST--
MongoDB\BSON\Regex unserialization does not allow pattern or flags to contain null bytes
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:18:"MongoDB\BSON\Regex":54:{a:2:{s:7:"pattern";s:7:"regexp' . "\0" . '";s:5:"flags";s:1:"i";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:18:"MongoDB\BSON\Regex":54:{a:2:{s:7:"pattern";s:6:"regexp";s:5:"flags";s:2:"i' . "\0" . '";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Pattern cannot contain null bytes
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Flags cannot contain null bytes
===DONE===

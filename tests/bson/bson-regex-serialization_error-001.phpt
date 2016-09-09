--TEST--
MongoDB\BSON\Regex unserialization requires "pattern" and "flags" string fields
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:18:"MongoDB\BSON\Regex":33:{a:1:{s:7:"pattern";s:6:"regexp";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:18:"MongoDB\BSON\Regex":26:{a:1:{s:5:"flags";s:1:"i";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:18:"MongoDB\BSON\Regex":40:{a:2:{s:7:"pattern";i:0;s:5:"flags";i:0;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex initialization requires "pattern" and "flags" string fields
===DONE===

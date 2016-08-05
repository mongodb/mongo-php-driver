--TEST--
MongoDB\BSON\Regex::__construct() does not allow pattern or flags to contain null bytes
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\Regex("regexp\0", 'i');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\Regex('regexp', "i\0");
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

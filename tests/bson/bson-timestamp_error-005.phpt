--TEST--
MongoDB\BSON\Timestamp constructor requires strings to parse as 64-bit integers
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\Timestamp('1.23', '5678');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\Timestamp('1234', '5.67');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "1.23" as 64-bit integer increment for MongoDB\BSON\Timestamp initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "5.67" as 64-bit integer timestamp for MongoDB\BSON\Timestamp initialization
===DONE===

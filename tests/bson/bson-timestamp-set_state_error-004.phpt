--TEST--
MongoDB\BSON\Timestamp::__set_state() requires strings to parse as 64-bit integers
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\BSON\Timestamp::__set_state(['increment' => '1.23', 'timestamp' => '5678']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Timestamp::__set_state(['increment' => '1234', 'timestamp' => '5.67']);
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

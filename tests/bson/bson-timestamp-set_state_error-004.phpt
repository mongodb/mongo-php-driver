--TEST--
MongoDB\BSON\Timestamp::__set_state() with broken numeric strings
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\BSON\Timestamp::__set_state(['increment' => 'broken', 'timestamp' => '5678']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Timestamp::__set_state(['increment' => '1234', 'timestamp' => 'broken']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "broken" as 64-bit integer increment for MongoDB\BSON\Timestamp initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "broken" as 64-bit integer timestamp for MongoDB\BSON\Timestamp initialization
===DONE===

--TEST--
MongoDB\Driver\Query construction (invalid maxAwaitTimeMS range)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Query([], ['maxAwaitTimeMS' => 4294967296]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "maxAwaitTimeMS" option to be <= 4294967295, 4294967296 given
===DONE===

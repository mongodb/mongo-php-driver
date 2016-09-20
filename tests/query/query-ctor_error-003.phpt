--TEST--
MongoDB\Driver\Query construction (negative limit conflicts with false singleBatch)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Query([], ['limit' => -1, 'singleBatch' => false]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Negative "limit" option conflicts with false "singleBatch" option
===DONE===

--TEST--
MongoDB\Driver\Query construction (invalid maxAwaitTimeMS range)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Query([], ['maxAwaitTimeMS' => -1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "maxAwaitTimeMS" option to be >= 0, -1 given
===DONE===

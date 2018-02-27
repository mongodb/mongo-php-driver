--TEST--
MongoDB\Driver\WriteConcern construction (invalid wtimeout range)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\WriteConcern(1, -1);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected wtimeout to be >= 0, -1 given
===DONE===

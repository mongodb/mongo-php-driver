--TEST--
MongoDB\Driver\Query construction (negative limit conflicts with false singleBatch)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\Driver\Query([], ['limit' => -1, 'singleBatch' => false]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Deprecated: MongoDB\Driver\Query::__construct(): Support for negative "limit" values is deprecated and will be removed in ext-mongodb 2.0 in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Negative "limit" option conflicts with false "singleBatch" option
===DONE===

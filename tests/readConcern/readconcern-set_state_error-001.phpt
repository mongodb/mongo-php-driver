--TEST--
MongoDB\Driver\ReadConcern::__set_state() requires "level" string field
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\Driver\ReadConcern::__set_state(['level' => 0]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadConcern initialization requires "level" string field
===DONE===

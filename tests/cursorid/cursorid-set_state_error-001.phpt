--TEST--
MongoDB\Driver\CursorId::__set_state() requires "id" string field
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\Driver\CursorId::__set_state(['id' => 0]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\CursorId initialization requires "id" string field
===DONE===

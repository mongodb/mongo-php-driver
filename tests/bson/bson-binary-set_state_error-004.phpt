--TEST--
MongoDB\BSON\Binary::__set_state() requires valid vector data
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\Binary::__set_state(['data' => '', 'type' => MongoDB\BSON\Binary::TYPE_VECTOR]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Binary vector data is invalid
===DONE===

--TEST--
MongoDB\BSON\Binary::__set_state() requires 16-byte data length for UUID types
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\BSON\Binary::__set_state(['data' => '0123456789abcde', 'type' => MongoDB\BSON\Binary::TYPE_OLD_UUID]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::__set_state(['data' => '0123456789abcdefg', 'type' => MongoDB\BSON\Binary::TYPE_OLD_UUID]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::__set_state(['data' => '0123456789abcde', 'type' => MongoDB\BSON\Binary::TYPE_UUID]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::__set_state(['data' => '0123456789abcdefg', 'type' => MongoDB\BSON\Binary::TYPE_UUID]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 15 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 17 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 15 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected UUID length to be 16 bytes, 17 given
===DONE===

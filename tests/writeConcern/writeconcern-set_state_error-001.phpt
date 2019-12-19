--TEST--
MongoDB\Driver\WriteConcern::__set_state() requires correct data types and values
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\Driver\WriteConcern::__set_state(['w' => -4]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\WriteConcern::__set_state(['w' => M_PI]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";


echo throws(function() {
    MongoDB\Driver\WriteConcern::__set_state(['wtimeout' => -1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\WriteConcern::__set_state(['wtimeout' => 'failure']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    MongoDB\Driver\WriteConcern::__set_state(['wtimeout' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";


echo throws(function() {
    MongoDB\Driver\WriteConcern::__set_state(['j' => 'failure']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\WriteConcern initialization requires "w" integer field to be >= -3
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\WriteConcern initialization requires "w" field to be integer or string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\WriteConcern initialization requires "wtimeout" integer field to be >= 0
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "failure" as 64-bit value for MongoDB\Driver\WriteConcern initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\WriteConcern initialization requires "wtimeout" field to be integer or string
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\WriteConcern initialization requires "j" field to be boolean
===DONE===

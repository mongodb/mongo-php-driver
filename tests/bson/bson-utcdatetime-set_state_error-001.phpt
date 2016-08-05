--TEST--
MongoDB\BSON\UTCDateTime::__set_state() requires "milliseconds" integer or numeric string field
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    MongoDB\BSON\UTCDateTime::__set_state(['milliseconds' => 1.0]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\UTCDateTime initialization requires "milliseconds" integer or numeric string field
===DONE===

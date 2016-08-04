--TEST--
MongoDB\BSON\UTCDateTime requires object argument to implement DateTimeInterface
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\UTCDateTime(new stdClass);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected instance of DateTimeInterface, stdClass given
===DONE===

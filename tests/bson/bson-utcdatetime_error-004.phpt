--TEST--
MongoDB\BSON\UTCDateTime constructor requires integer or string argument
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$invalidValues = [true, []];

foreach ($invalidValues as $invalidValue) {
    echo throws(function() use ($invalidValue) {
        new MongoDB\BSON\UTCDateTime($invalidValue);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or string, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or string, array given
===DONE===

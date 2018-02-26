--TEST--
MongoDB\BSON\Timestamp #001 error
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

throws(function() {
    new MongoDB\BSON\Timestamp;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===


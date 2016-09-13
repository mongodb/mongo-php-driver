--TEST--
MongoDB\BSON\UTCDateTime unserialization requires "milliseconds" integer or numeric string field
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:24:"MongoDB\BSON\UTCDateTime":30:{a:1:{s:12:"milliseconds";d:1;}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\UTCDateTime initialization requires "milliseconds" integer or numeric string field
===DONE===

--TEST--
MongoDB\BSON\ObjectID unserialization requires valid hex string
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    unserialize('C:21:"MongoDB\BSON\ObjectID":48:{a:1:{s:3:"oid";s:24:"0123456789abcdefghijklmn";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    unserialize('C:21:"MongoDB\BSON\ObjectID":30:{a:1:{s:3:"oid";s:7:"INVALID";}}');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing ObjectID string: 0123456789abcdefghijklmn
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing ObjectID string: INVALID
===DONE===

--TEST--
MongoDB\BSON\ObjectID::__construct() requires valid hex string
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\ObjectID('0123456789abcdefghijklmn');
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\ObjectID('INVALID');
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

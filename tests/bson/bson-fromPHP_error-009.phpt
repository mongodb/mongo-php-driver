--TEST--
MongoDB\BSON\fromPHP(): PackedArray cannot be serialized as root document
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    fromPHP(MongoDB\BSON\PackedArray::fromPHP([]));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
===DONE===

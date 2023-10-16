--TEST--
MongoDB\Driver\Command::__construct() does not allow PackedArray
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    new MongoDB\Driver\Command(MongoDB\BSON\PackedArray::fromPHP([]));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
===DONE===

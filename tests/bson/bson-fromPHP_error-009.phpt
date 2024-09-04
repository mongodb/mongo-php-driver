--TEST--
MongoDB\BSON\fromPHP(): PackedArray cannot be serialized as root document
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\fromPHP(MongoDB\BSON\PackedArray::fromPHP([]));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: Function MongoDB\BSON\fromPHP() is deprecated in %s
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
===DONE===

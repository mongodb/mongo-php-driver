--TEST--
Non-backed enums cannot be serialized
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum
{
    case A;
}

echo throws(function() {
    fromPHP(['x' => MyEnum::A]);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Non-backed enum MyEnum cannot be serialized for field path "x"
===DONE===

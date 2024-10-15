--TEST--
Enums cannot be serialized as a root element
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum
{
    case A;
}

enum MyBackedEnum: int
{
    case A = 1;
}

echo throws(function() {
    fromPHP(MyEnum::A);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() {
    fromPHP(MyBackedEnum::A);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Enum MyEnum cannot be serialized as a root element
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Enum MyBackedEnum cannot be serialized as a root element
===DONE===

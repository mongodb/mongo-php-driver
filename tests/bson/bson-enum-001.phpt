--TEST--
Backed enums serialize as their case value
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyIntBackedEnum: int
{
    case A = 1;
}

enum MyStringBackedEnum: string
{
    case A = 'a';
}

$document = [
    'x' => MyIntBackedEnum::A,
    'y' => MyStringBackedEnum::A,
];

echo toCanonicalExtendedJSON(fromPHP($document)), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{ "x" : { "$numberInt" : "1" }, "y" : "a" }
===DONE===

--TEST--
Backed enums serialize as their case value
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
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

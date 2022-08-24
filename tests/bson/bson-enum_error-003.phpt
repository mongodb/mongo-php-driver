--TEST--
Typemap specifies enum that does not implement Unserializable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum
{
    const NOT_A_CASE = 1;
    case foo;
}

echo throws(function() {
    toPHP(
        fromJSON('{}'),
        ['root' => MyEnum::class]
    );
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    toPHP(
        fromJSON('{"name": "NOT_A_CONSTANT"}'),
        ['root' => MyEnum::class]
    );
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    toPHP(
        fromJSON('{"name": "NOT_A_CASE"}'),
        ['root' => MyEnum::class]
    );
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    toPHP(
        fromJSON('{"myEnum": {}}'),
        ['fieldPaths' => ['myEnum' => MyEnum::class]]
    );
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    toPHP(
        fromJSON('{"myEnum": {"name": "NOT_A_CONSTANT"}}'),
        ['fieldPaths' => ['myEnum' => MyEnum::class]]
    );
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    toPHP(
        fromJSON('{"myEnum": {"name": "NOT_A_CASE"}}'),
        ['fieldPaths' => ['myEnum' => MyEnum::class]]
    );
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyEnum does not implement MongoDB\BSON\Unserializable
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyEnum does not implement MongoDB\BSON\Unserializable
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyEnum does not implement MongoDB\BSON\Unserializable
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyEnum does not implement MongoDB\BSON\Unserializable
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyEnum does not implement MongoDB\BSON\Unserializable
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Class MyEnum does not implement MongoDB\BSON\Unserializable
===DONE===

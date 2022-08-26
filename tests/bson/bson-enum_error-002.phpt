--TEST--
Unserialization errors for enums implementing Persistable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

enum MyEnum implements MongoDB\BSON\Persistable
{
    const NOT_A_CASE = 1;
    case foo;

    public function bsonSerialize(): array
    {
        return (array) $this;
    }

    public function bsonUnserialize(array $data): void
    {
        printf("%s called with: %s\n", __METHOD__, json_encode($data));
    }
}

echo throws(function() {
    toPHP(fromJSON('{"__pclass": {"$binary": "TXlFbnVt", "$type": "80"}}'));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() {
    toPHP(fromJSON('{"__pclass": {"$binary": "TXlFbnVt", "$type": "80"}, "name": "NOT_A_CONSTANT"}'));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() {
    toPHP(fromJSON('{"__pclass": {"$binary": "TXlFbnVt", "$type": "80"}, "name": "NOT_A_CASE"}'));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() {
    toPHP(fromJSON('{"myEnum": {"__pclass": {"$binary": "TXlFbnVt", "$type": "80"}}}'));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() {
    toPHP(fromJSON('{"myEnum": {"__pclass": {"$binary": "TXlFbnVt", "$type": "80"}, "name": "NOT_A_CONSTANT"}}'));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() {
    toPHP(fromJSON('{"myEnum": {"__pclass": {"$binary": "TXlFbnVt", "$type": "80"}, "name": "NOT_A_CASE"}}'));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Missing 'name' field to infer enum case for MyEnum
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Undefined constant MyEnum::NOT_A_CONSTANT
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MyEnum::NOT_A_CASE is not an enum case
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Missing 'name' field to infer enum case for MyEnum
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Undefined constant MyEnum::NOT_A_CONSTANT
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MyEnum::NOT_A_CASE is not an enum case
===DONE===

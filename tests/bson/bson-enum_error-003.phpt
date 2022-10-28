--TEST--
Enum cannot implement Persistable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.1.0'); ?>
--FILE--
<?php

enum MyEnum implements MongoDB\BSON\Persistable
{
    case A;

    public function bsonSerialize(): array {}

    public function bsonUnserialize(array $data): void {}
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Enum class MyEnum cannot implement interface MongoDB\BSON\Persistable in %s on line %d

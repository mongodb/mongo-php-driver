--TEST--
Enum cannot implement Persistable
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

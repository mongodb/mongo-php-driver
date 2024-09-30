--TEST--
Enum cannot implement Unserializable
--FILE--
<?php

enum MyEnum implements MongoDB\BSON\Unserializable
{
    case A;

    public function bsonUnserialize(array $data): void {}
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Enum class MyEnum cannot implement interface MongoDB\BSON\Unserializable in %s on line %d

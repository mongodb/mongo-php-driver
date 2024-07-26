--TEST--
Backed enum cannot implement Unserializable
--FILE--
<?php

enum MyBackedEnum: int implements MongoDB\BSON\Unserializable
{
    case A = 1;

    public function bsonUnserialize(array $data): void {}
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Enum class MyBackedEnum cannot implement interface MongoDB\BSON\Unserializable in %s on line %d

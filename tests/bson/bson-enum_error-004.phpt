--TEST--
Backed enum cannot implement Persistable
--FILE--
<?php

enum MyBackedEnum: int implements MongoDB\BSON\Persistable
{
    case A = 1;

    public function bsonSerialize(): array {}

    public function bsonUnserialize(array $data): void {}
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTREGEX--
Fatal error: Enum class MyBackedEnum cannot implement interface MongoDB\\BSON\\(Persistable|Unserializable) in .+ on line \d+

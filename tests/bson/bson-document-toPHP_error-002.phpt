--TEST--
MongoDB\BSON\Document::toPHP(): exception thrown in Unserializable::bsonUnserialize() propagates (root document)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

class ThrowingClass implements MongoDB\BSON\Unserializable
{
    public function bsonUnserialize(array $data): void
    {
        throw new RuntimeException('bsonUnserialize threw');
    }
}

$bson = MongoDB\BSON\Document::fromPHP(['foo' => 'bar', 'baz' => 42]);

echo throws(function() use ($bson) {
    $bson->toPHP(['root' => ThrowingClass::class]);
}, RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got RuntimeException
bsonUnserialize threw
===DONE===

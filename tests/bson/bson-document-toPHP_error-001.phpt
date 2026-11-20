--TEST--
MongoDB\BSON\Document::toPHP(): exception thrown in Unserializable::bsonUnserialize() propagates (embedded document)
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

$bson = MongoDB\BSON\Document::fromPHP([
    'foo' => 'bar',
    'nested' => ['a' => 1, 'b' => 2],
]);

echo throws(function() use ($bson) {
    $bson->toPHP(['document' => ThrowingClass::class]);
}, RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got RuntimeException
bsonUnserialize threw
===DONE===

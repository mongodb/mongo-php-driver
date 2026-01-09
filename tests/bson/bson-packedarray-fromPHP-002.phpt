--TEST--
MongoDB\BSON\PackedArray::fromPHP() respects nesting limit
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

function createNestedArray(int $levels): array
{
    $value = 10;
    for ($i = 0; $i <= $levels; $i++) {
        $value = [$value];
    }

    return $value;
}

echo "Creating packed array with 100 levels of nesting\n";
MongoDB\BSON\PackedArray::fromPHP(createNestedArray(100));

echo "Creating packed array with 101 levels of nesting\n";
echo throws(function() {
    MongoDB\BSON\PackedArray::fromPHP(createNestedArray(101));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Creating packed array with 100 levels of nesting
Creating packed array with 101 levels of nesting
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep
===DONE===

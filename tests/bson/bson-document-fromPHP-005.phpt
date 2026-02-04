--TEST--
MongoDB\BSON\Document::fromPHP() respects nesting limit for BSON objects (from array)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

function createNestedArray(int $levels): array
{
    $value = 10;
    for ($i = 0; $i <= $levels; $i++) {
        $value = ['nested' => $value];
    }

    return $value;
}

echo "Creating document with 100 levels of nesting\n";
MongoDB\BSON\Document::fromPHP(createNestedArray(100));

echo "Creating document with 101 levels of nesting\n";
echo throws(function() {
    MongoDB\BSON\Document::fromPHP(createNestedArray(101));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Creating document with 100 levels of nesting
Creating document with 101 levels of nesting
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Nesting level too deep
===DONE===

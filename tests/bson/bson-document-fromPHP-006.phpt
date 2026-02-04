--TEST--
MongoDB\BSON\Document::fromPHP() respects nesting limit for BSON objects (from object)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

function createNestedObject(int $levels): object
{
    $value = 10;
    for ($i = 0; $i <= $levels; $i++) {
        $value = (object) ['nested' => $value];
    }

    return $value;
}

echo "Creating document with 100 levels of nesting\n";
MongoDB\BSON\Document::fromPHP(createNestedObject(100));

echo "Creating document with 101 levels of nesting\n";
echo throws(function() {
    MongoDB\BSON\Document::fromPHP(createNestedObject(101));
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

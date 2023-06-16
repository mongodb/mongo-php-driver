--TEST--
MongoDB\BSON\Iterator does not extend past the last element in a structure
--FILE--
<?php

require __DIR__ . "/../utils/basic.inc";

function showIteratorStatus(MongoDB\BSON\Iterator $iterator): void
{
    var_dump($iterator->valid());
    var_dump($iterator->current());
    var_dump($iterator->key());
}

$array = MongoDB\BSON\PackedArray::fromPHP([10, 20]);
$iterator = $array->getIterator();

showIteratorStatus($iterator);
$iterator->next();
showIteratorStatus($iterator);

// Will take the iterator to an invalid state
$iterator->next();

var_dump($iterator->valid());

echo throws(function () use ($iterator) {
    $iterator->current();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

echo throws(function () use ($iterator) {
    $iterator->key();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
int(10)
int(0)
bool(true)
int(20)
int(1)
bool(false)
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call current() on an exhausted iterator
OK: Got MongoDB\Driver\Exception\LogicException
Cannot call key() on an exhausted iterator
===DONE===

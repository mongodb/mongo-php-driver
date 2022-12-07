--TEST--
MongoDB\BSON\Iterator does not extend past the last element in a structure
--FILE--
<?php

function showIteratorStatus(MongoDB\BSON\Iterator $iterator): void
{
    var_dump($iterator->valid());
    var_dump($iterator->current());
    var_dump($iterator->key());
}

$array = MongoDB\BSON\ArrayList::fromPHP([10, 20]);
$iterator = $array->getIterator();

showIteratorStatus($iterator);
$iterator->next();
showIteratorStatus($iterator);
$iterator->next();
showIteratorStatus($iterator);

// Will never be executed as the code errors earlier
$iterator->next();
showIteratorStatus($iterator);

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
NULL

Fatal error: MongoDB\BSON\Iterator::key(): Return value must be of type string|int, null returned%s

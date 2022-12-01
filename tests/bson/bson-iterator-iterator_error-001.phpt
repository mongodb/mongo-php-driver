--TEST--
MongoDB\BSON\Iterator cannot be rewound
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$array = MongoDB\BSON\Document::fromPHP([0, 1, "foo"]);
$iterator = $array->getIterator();
// No error as long as the iterator wasn't advanced
var_dump($iterator->current());
$iterator->rewind();
var_dump($iterator->current());
$iterator->next();
var_dump($iterator->current());
echo throws(function () use ($iterator): void {
    $iterator->rewind();
}, MongoDB\Driver\Exception\LogicException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
int(0)
int(1)
OK: Got MongoDB\Driver\Exception\LogicException
BSON iterators cannot rewind after starting iteration
===DONE===

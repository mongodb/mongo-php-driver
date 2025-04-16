--TEST--
MongoDB\Driver\BulkWriteCommand::updateOne() with invalid options
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bulk = new MongoDB\Driver\BulkWriteCommand;

// @TODO: Invalid key 'y': update only works with $ operators and pipelines
echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ['y' => 1], ['multi' => true]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ['y' => 1], ['collation' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ['$set' => ['y' => 1]], ['collation' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ['$set' => ['y' => 1]], ['arrayFilters' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

// @TODO: Expected "arrayFilters" option to yield array but got non-sequential keys
echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ['$set' => ['y' => 1]], ['arrayFilters' => ['foo' => 'bar']]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ['$set' => ['y' => 1]], ['hint' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Replacement document conflicts with true "multi" option

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "collation" option to be array or object, int given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "collation" option to be array or object, int given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "arrayFilters" option to be array or object, int given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
"arrayFilters" option has invalid keys for a BSON array

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to be string, array, or object, int given
===DONE===

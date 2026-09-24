--TEST--
MongoDB\Driver\BulkWriteCommand::deleteMany() with invalid options
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bulk = new MongoDB\Driver\BulkWriteCommand;

echo throws(function() use ($bulk) {
    $bulk->deleteMany(NS, ['x' => 1], ['collation' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->deleteMany(NS, ['x' => 1], ['hint' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "collation" option to be array or object, int given

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to be string, array, or object, int given
===DONE===

--TEST--
MongoDB\Driver\BulkWriteCommand::updateOne() with invalid replacement document
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bulk = new MongoDB\Driver\BulkWriteCommand;

// @TODO: Invalid key '': update only works with $ operators and pipelines
echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ['' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

// @TODO: Invalid key '�(': update only works with $ operators and pipelines
echo throws(function() use ($bulk) {
    $bulk->updateOne(NS, ['x' => 1], ["\xc3\x28" => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
invalid argument for replace: empty key

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
invalid argument for replace: corrupt BSON
===DONE===
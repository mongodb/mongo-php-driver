--TEST--
MongoDB\Driver\BulkWriteCommand::insertOne() with invalid insert document
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bulk = new MongoDB\Driver\BulkWriteCommand;

// @TODO: FAILED: Expected MongoDB\Driver\Exception\InvalidArgumentException, but no exception thrown!
echo throws(function() use ($bulk) {
    $bulk->insertOne(NS, ['' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

// @TODO: FAILED: Expected MongoDB\Driver\Exception\InvalidArgumentException, but no exception thrown!
echo throws(function() use ($bulk) {
    $bulk->insertOne(NS, ["\xc3\x28" => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
invalid document for insert: empty key

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
invalid document for insert: corrupt BSON
===DONE===

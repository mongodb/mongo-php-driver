--TEST--
MongoDB\Driver\BulkWrite::insert() with invalid insert document
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->insert(['' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->insert(["\xc3\x28" => 1]);
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

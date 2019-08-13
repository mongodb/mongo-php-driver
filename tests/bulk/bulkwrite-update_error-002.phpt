--TEST--
MongoDB\Driver\BulkWrite::update() with invalid update document
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['$set' => ['x' => ['' => 1]]]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['$set' => ['x' => ["\xc3\x28" => 1]]]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

/* This newObj argument mixes an update and replacement document, but
 * php_phongo_bulkwrite_update_has_operators() will categorize it as an update
 * due to the presence of an atomic operator. As such, _mongoc_validate_update()
 * will report the error. */
echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['$set' => ['y' => 1], 'z' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
invalid argument for update: empty key

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
invalid argument for update: corrupt BSON

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid key 'z': update only works with $ operators and pipelines
===DONE===
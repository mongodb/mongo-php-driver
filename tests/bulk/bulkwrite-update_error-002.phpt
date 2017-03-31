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
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
update document contains invalid keys

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
update document contains invalid keys
===DONE===
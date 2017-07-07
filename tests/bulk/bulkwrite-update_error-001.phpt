--TEST--
MongoDB\Driver\BulkWrite::update() with invalid replacement document
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['x.y' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['y' => ['$x' => 1]]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ["\xc3\x28" => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
replacement document contains invalid key: empty key

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
replacement document contains invalid key: keys cannot contain ".": "x.y"

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
replacement document contains invalid key: keys cannot begin with "$": "$x"

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
replacement document contains invalid key: corrupt BSON
===DONE===
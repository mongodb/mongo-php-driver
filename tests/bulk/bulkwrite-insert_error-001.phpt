--TEST--
MongoDB\Driver\BulkWrite::insert() with invalid insert document
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->insert(['' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->insert(['x.y' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->insert(['$x' => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->insert(["\xc3\x28" => 1]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
document to insert contains invalid key: empty key

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
document to insert contains invalid key: keys cannot contain ".": "x.y"

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
document to insert contains invalid key: keys cannot begin with "$": "$x"

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
document to insert contains invalid key: corrupt BSON
===DONE===

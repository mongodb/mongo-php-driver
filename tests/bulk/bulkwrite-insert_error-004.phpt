--TEST--
MongoDB\Driver\BulkWrite::insert() with invalid insert document (legacy index)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->insert(['' => 1, 'key' => ['a.b' => 1], 'name' => 'a.b_1', 'ns' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->insert(['$x' => 1, 'key' => ['a.b' => 1], 'name' => 'a.b_1', 'ns' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->insert(["\xc3\x28" => 1, 'key' => ['a.b' => 1], 'name' => 'a.b_1', 'ns' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
legacy index document contains invalid key: empty key

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
legacy index document contains invalid key: keys cannot begin with "$": "$x"

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
legacy index document contains invalid key: corrupt BSON
===DONE===

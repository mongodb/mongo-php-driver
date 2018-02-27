--TEST--
MongoDB\Driver\BulkWrite::update() with BSON encoding error (invalid UTF-8 string)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->update(['x' => "\xc3\x28"], ['x' => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['x' => "\xc3\x28"]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['$set' => ['x' => "\xc3\x28"]]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->update(['x' => 1], ['y' => 1], ['collation' => ['locale' => "\xc3\x28"]]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected invalid UTF-8 for field path "x": %s

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected invalid UTF-8 for field path "x": %s

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected invalid UTF-8 for field path "$set.x": %s

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Detected invalid UTF-8 for field path "locale": %s
===DONE===

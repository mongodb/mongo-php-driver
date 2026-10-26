--TEST--
MongoDB\Driver\BulkWriteCommand::deleteOne() with BSON encoding error (null bytes in keys)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bulk = new MongoDB\Driver\BulkWriteCommand;

echo throws(function() use ($bulk) {
    $bulk->deleteOne(NS, ["\0" => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->deleteOne(NS, ["x\0" => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->deleteOne(NS, ["\0\0\0" => 1]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->deleteOne(NS, ['x' => 1], ['collation' => ["\0" => 1]]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->deleteOne(NS, ['x' => 1], ['collation' => ["x\0" => 1]]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n\n";

echo throws(function() use ($bulk) {
    $bulk->deleteOne(NS, ['x' => 1], ['collation' => ["\0\0\0" => 1]]);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "x".

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "x".

OK: Got MongoDB\Driver\Exception\UnexpectedValueException
BSON keys cannot contain null bytes. Unexpected null byte after "".
===DONE===

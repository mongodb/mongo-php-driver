--TEST--
MongoDB\Driver\BulkWrite::update() arrayFilters option requires MongoDB 3.6
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('>=', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();

$bulk->update(
    ['grades' => ['$gte' => 100]],
    ['$set' => ['grades.$[element]' => 100 ]],
    [
        'arrayFilters' => [['element' => ['$gte' => 100]]],
        'multi' => true,
    ]
);

echo throws(function() use ($manager, $bulk) {
    $manager->executeBulkWrite(DATABASE_NAME . '.' . COLLECTION_NAME, $bulk);
}, 'MongoDB\Driver\Exception\BulkWriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\BulkWriteException
Bulk write failed due to previous MongoDB\Driver\Exception\RuntimeException: The selected server does not support array filters
===DONE===

--TEST--
MongoDB\Driver\BulkWrite::update() hint option requires MongoDB 4.2
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('>=', '4.2'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['_id' => 1], ['$set' => ['x' => 11]], ['hint' => '_id_']);

echo throws(function() use ($manager, $bulk) {
    $manager->executeBulkWrite(NS, $bulk);
}, 'MongoDB\Driver\Exception\BulkWriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\BulkWriteException
Bulk write failed due to previous MongoDB\Driver\Exception\RuntimeException: The selected server does not support hint for update
===DONE===

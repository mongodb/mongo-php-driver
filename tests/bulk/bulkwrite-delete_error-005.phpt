--TEST--
MongoDB\Driver\BulkWrite::delete() hint option requires MongoDB 4.4 (server-side error)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('>=', '4.3.4'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->delete(['_id' => 1], ['hint' => '_id_']);

echo throws(function() use ($manager, $bulk) {
    $manager->executeBulkWrite(NS, $bulk);
}, 'MongoDB\Driver\Exception\BulkWriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\BulkWriteException
BSON field 'delete.deletes.hint' is an unknown field.
===DONE===

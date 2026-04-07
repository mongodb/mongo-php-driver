--TEST--
MongoDB\Driver\Exception\BulkWriteCommandException::getWriteConcernErrors()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

/* Trigger a write error (duplicate key) with no write concern error */
$bulk = new MongoDB\Driver\BulkWriteCommand(['ordered' => false]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 1]);

try {
    $manager->executeBulkWriteCommand($bulk);
} catch (MongoDB\Driver\Exception\BulkWriteCommandException $e) {
    var_dump($e->getWriteConcernErrors());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(0) {
}
===DONE===

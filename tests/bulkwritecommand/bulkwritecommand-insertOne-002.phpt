--TEST--
MongoDB\Driver\BulkWriteCommand::insertOne() $document is MongoDB\BSON\Document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$document = MongoDB\BSON\Document::fromJSON('{ "_id": 2 }');

$bulk = new MongoDB\Driver\BulkWriteCommand();
$insertedId = $bulk->insertOne(NS, $document);
$result = $manager->executeBulkWriteCommand($bulk);

var_dump($insertedId);
var_dump($result->getInsertedCount());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(2)
int(1)
===DONE===

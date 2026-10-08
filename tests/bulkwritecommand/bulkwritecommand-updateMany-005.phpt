--TEST--
MongoDB\Driver\BulkWriteCommand::updateMany() $filter and $newObj are MongoDB\BSON\Document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand();
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 2]);
$manager->executeBulkWriteCommand($bulk);

$filter = MongoDB\BSON\Document::fromJSON('{ "_id": { "$gt": 1 } }');
$newObj = MongoDB\BSON\Document::fromJSON('{ "$set": { "x": 1 } }');

$bulk = new MongoDB\Driver\BulkWriteCommand;
$bulk->updateMany(NS, $filter, $newObj);
$result = $manager->executeBulkWriteCommand($bulk);

var_dump($result->getMatchedCount());
var_dump($result->getModifiedCount());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(1)
int(1)
===DONE===

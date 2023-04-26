--TEST--
MongoDB\Driver\BulkWrite::update() $filter and $newObj are MongoDB\BSON\Document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$manager->executeBulkWrite(NS, $bulk);

$filter = MongoDB\BSON\Document::fromJSON('{ "_id": { "$gt": 1 } }');
$newObj = MongoDB\BSON\Document::fromJSON('{ "$set": { "x": 1 } }');

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update($filter, $newObj);
$result = $manager->executeBulkWrite(NS, $bulk);

var_dump($result->getMatchedCount());
var_dump($result->getModifiedCount());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(1)
int(1)
===DONE===

--TEST--
MongoDB\Driver\WriteResult::getUpsertedIds() with server-generated values
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$set' => ['y' => 3]]);
$bulk->update(['x' => 2], ['$set' => ['y' => 1]], ['upsert' => true]);
$bulk->update(['x' => 3], ['$set' => ['y' => 2]], ['upsert' => true]);
$bulk->delete(['x' => 1]);

$result = $manager->executeBulkWrite(NS, $bulk);

var_dump($result->getUpsertedIds());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  [2]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%x"
  }
  [3]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%x"
  }
}
===DONE===

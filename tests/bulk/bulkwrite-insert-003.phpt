--TEST--
MongoDB\Driver\BulkWrite::insert() with legacy index false positive (2.6+ server)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* Note: 2.6+ servers use the createIndexes command instead of inserting to a
 * "system.indexes" collection. The purpose of this test is to ensure that a
 * false positive in our legacy index detection does not interfere with inserts
 * for newer servers. Although the driver will set libmongoc's "legacyIndex"
 * option and allow dots in BSON keys, we are not testing the server's BSON
 * validation here and will only attempt to insert a valid document. */
$manager = new MongoDB\Driver\Manager(STANDALONE);

$legacyIndex = [
    'key' => ['a' => 1], // Do not attempt to use dots in BSON keys
    'name' => 'a_1',
    'ns' => NS,
];

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert($legacyIndex);
$result = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(['name' => 'a_1']));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 1 document(s)
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
    ["key"]=>
    object(stdClass)#%d (%d) {
      ["a"]=>
      int(1)
    }
    ["name"]=>
    string(3) "a_1"
    ["ns"]=>
    string(32) "phongo.bulk_bulkwrite_insert_003"
  }
}
===DONE===

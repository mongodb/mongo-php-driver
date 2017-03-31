--TEST--
MongoDB\Driver\BulkWrite::insert() with legacy index (pre-2.6 server)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE_24'); CLEANUP(STANDALONE_24); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE_24);

$legacyIndex = [
    'key' => ['a.b' => 1],
    'name' => 'a.b_1',
    'ns' => NS,
];

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert($legacyIndex);
$result = $manager->executeBulkWrite(DATABASE_NAME . '.system.indexes', $bulk);
printf("Created %d index(es)\n", $result->getInsertedCount());

$cursor = $manager->executeQuery(DATABASE_NAME . '.system.indexes', new MongoDB\Driver\Query(['name' => 'a.b_1']));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Created 1 index(es)
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["v"]=>
    int(1)
    ["name"]=>
    string(5) "a.b_1"
    ["key"]=>
    object(stdClass)#%d (%d) {
      ["a.b"]=>
      int(1)
    }
    ["ns"]=>
    string(%d) "%s"
  }
}
===DONE===

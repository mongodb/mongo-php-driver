--TEST--
MongoDB\Manager::executeUpdate() multiple documents with no upsert
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

// load fixtures for test
$manager->executeInsert(array('_id' => 1, 'x' => 1));
$manager->executeInsert(array('_id' => 2, 'x' => 1));
$manager->executeInsert(array('_id' => 3, 'x' => 3));

$result = $manager->executeUpdate(
    NS,
    array('x' => 1),
    array('$set' => array('x' => 2)),
    array('multi' => true, 'upsert' => false)
);

var_dump($result instanceof MongoDB\Write\WriteResult);

$server = $result->getServer();

var_dump($server instanceof MongoDB\Server);
var_dump($server->getHost());
var_dump($server->getPort());

printf("Inserted: %d\n", $result->getNumInserted());
printf("Matched: %d\n", $result->getNumMatched());
printf("Modified: %d\n", $result->getNumModified());
printf("Upserted: %d\n", $result->getNumUpserted());
printf("Removed: %d\n", $result->getNumRemoved());
printf("Write concern errors: %d\n", count($result->getWriteConcernErrors()));
printf("Write errors: %d\n", count($result->getWriteErrors()));

$query = new MongoDB\Query\Query(array(), array(), null, 0, 0);
$cursor = $manager->executeQuery(NS, $query);

var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
string(9) "localhost"
int(27017)
Inserted: 0
Matched: 2
Modified: 2
Upserted: 0
Removed: 0
Write concern errors: 0
Write errors: 0
array(3) {
  [0] =>
  array(2) {
    '_id' =>
    int(1)
    'x' =>
    int(2)
  }
  [1] =>
  array(2) {
    '_id' =>
    int(2)
    'x' =>
    int(2)
  }
  [2] =>
  array(2) {
    '_id' =>
    int(3)
    'x' =>
    int(3)
  }
}
===DONE===

--TEST--
MongoDB\Server::executeWrite()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$server = new MongoDB\Server('localhost', 27017);

$batch = new MongoDB\Write\Batch();
$batch->insert(array('_id' => 1, 'x' => 1));
$batch->insert(array('_id' => 2, 'x' => 2));
$batch->update(array('x' => 2), array('$set' => array('x' => 1)), 1, false);
$batch->update(array('_id' => 3), array('$set' => array('x' => 3)), 1, true);
$batch->delete(array('x' => 1), 1);

$result = $server->executeWrite(NS, $batch);

var_dump($result instanceof MongoDB\Write\WriteResult);
var_dump($server === $result->getServer());

printf("Inserted: %d\n", $result->getNumInserted());
printf("Matched: %d\n", $result->getNumMatched());
printf("Modified: %d\n", $result->getNumModified());
printf("Upserted: %d\n", $result->getNumUpserted());
printf("Removed: %d\n", $result->getNumRemoved());
printf("Write concern errors: %d\n", count($result->getWriteConcernErrors()));
printf("Write errors: %d\n", count($result->getWriteErrors()));

$query = new MongoDB\Query\Query(array(), array(), null, 0, 0);
$cursor = $server->executeQuery(NS, $query);

var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
Inserted: 2
Matched: 1
Modified: 1
Upserted: 1
Removed: 1
Write concern errors: 0
Write errors: 0
array(2) {
  [0] =>
  array(2) {
    '_id' =>
    int(2)
    'x' =>
    int(1)
  }
  [1] =>
  array(2) {
    '_id' =>
    int(3)
    'x' =>
    int(3)
  }
}
===DONE===

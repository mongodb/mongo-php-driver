--TEST--
MongoDB\Server::executeQuery()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$server = new MongoDB\Server('localhost', 27017);

// load fixtures for test
$command = new MongoDB\Command(array('drop' => COLLECTION_NAME));
$server->executeCommand(DATABASE_NAME, $command);

$batch = new \MongoDB\WriteBatch();
$batch->insert(array('_id' => 1, 'x' => 2, 'y' => 3));
$batch->insert(array('_id' => 2, 'x' => 3, 'y' => 4));
$batch->insert(array('_id' => 3, 'x' => 4, 'y' => 5));
$server->executeWrite(NS, $batch);

$query = new MongoDB\Query(array('x' => 3), array('y' => 1), null, 0, 0);
$cursor = $server->executeQuery(NS, $query);

var_dump($cursor instanceof MongoDB\QueryCursor);
var_dump($server === $cursor->getServer());
var_dump(iterator_to_array($server));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
array(1)
  [0] =>
  array(2) {
    '_id' =>
    int(2)
    'y' =>
    int(4)
  }
}
===DONE===

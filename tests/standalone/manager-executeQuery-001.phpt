--TEST--
MongoDB\Manager::executeQuery() one document
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

// load fixtures for test
$batch = new MongoDB\WriteBatch();
$batch->insert(array('_id' => 1, 'x' => 2, 'y' => 3));
$batch->insert(array('_id' => 2, 'x' => 3, 'y' => 4));
$batch->insert(array('_id' => 3, 'x' => 4, 'y' => 5));
$manager->executeWriteBatch(NS, $batch);

$query = new MongoDB\Query(array('x' => 3), array('projection' => array('y' => 1)));
$cursor = $manager->executeQuery(NS, $query);

var_dump($cursor instanceof MongoDB\QueryResult);

$server = $cursor->getServer();

var_dump($server instanceof MongoDB\Server);
var_dump($server->getHost());
var_dump($server->getPort());

var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
string(9) "localhost"
int(27017)
array(1) {
  [0]=>
  array(2) {
    ["_id"]=>
    int(2)
    ["y"]=>
    int(4)
  }
}
===DONE===

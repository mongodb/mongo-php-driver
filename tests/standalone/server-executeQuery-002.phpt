--TEST--
MongoDB\Driver\Server::executeQuery() with sort and empty filter
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$parsed = parse_url(MONGODB_URI);
$server = new MongoDB\Driver\Server($parsed["host"], $parsed["port"]);

// load fixtures for test
$batch = new \MongoDB\Driver\WriteBatch();
$batch->insert(array('_id' => 1, 'x' => 2, 'y' => 3));
$batch->insert(array('_id' => 2, 'x' => 3, 'y' => 4));
$batch->insert(array('_id' => 3, 'x' => 4, 'y' => 5));
$server->executeWriteBatch(NS, $batch);

$query = new MongoDB\Driver\Query(array(), array('sort' => array('_id' => -1)));
$cursor = $server->executeQuery(NS, $query);

var_dump($cursor instanceof MongoDB\Driver\QueryResult);
var_dump($server == $cursor->getServer());
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
array(3) {
  [0]=>
  array(3) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(4)
    ["y"]=>
    int(5)
  }
  [1]=>
  array(3) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(3)
    ["y"]=>
    int(4)
  }
  [2]=>
  array(3) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(2)
    ["y"]=>
    int(3)
  }
}
===DONE===

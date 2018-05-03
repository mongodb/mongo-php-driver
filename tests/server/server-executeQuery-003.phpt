--TEST--
MongoDB\Driver\Server::executeQuery() with modifiers and empty filter
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()))->getServer();

// load fixtures for test
$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 2, 'y' => 3));
$bulk->insert(array('_id' => 2, 'x' => 3, 'y' => 4));
$bulk->insert(array('_id' => 3, 'x' => 4, 'y' => 5));
$server->executeBulkWrite(NS, $bulk);

$query = new MongoDB\Driver\Query(array(), array('modifiers' => array('$comment' => 'foo')));
$cursor = $server->executeQuery(NS, $query);

var_dump($cursor instanceof MongoDB\Driver\Cursor);
var_dump($server == $cursor->getServer());
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
array(3) {
  [0]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(2)
    ["y"]=>
    int(3)
  }
  [1]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(3)
    ["y"]=>
    int(4)
  }
  [2]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(4)
    ["y"]=>
    int(5)
  }
}
===DONE===

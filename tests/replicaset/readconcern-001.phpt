--TEST--
ReadConcern: MongoDB\Driver\Manager::executeQuery() with readConcern option (find command)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1, 'x' => 1]);
$bulk->insert(['_id' => 2, 'x' => 2]);
$manager->executeBulkWrite(NS, $bulk, $wc);

$rc = new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL);
$query = new MongoDB\Driver\Query(['x' => 2], ['readConcern' => $rc]);
$cursor = $manager->executeQuery(NS, $query);

var_dump(iterator_to_array($cursor));

$rc = new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY);
$query = new MongoDB\Driver\Query(['x' => 2], ['readConcern' => $rc]);
$cursor = $manager->executeQuery(NS, $query);

var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(2)
  }
}
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(2)
  }
}
===DONE===

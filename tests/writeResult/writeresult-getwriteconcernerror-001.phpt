--TEST--
MongoDB\Driver\WriteResult::getWriteConcernError()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);

try {
    /* We assume that the replica set does not have 12 nodes */
    $result = $manager->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern(12));
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult()->getWriteConcernError());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcernError)#%d (%d) {
  ["message"]=>
  string(29) "Not enough data-bearing nodes"
  ["code"]=>
  int(100)
  ["info"]=>
  NULL
}
===DONE===

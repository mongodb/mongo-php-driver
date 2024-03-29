--TEST--
MongoDB\Driver\Session::getClusterTime()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_sharded_cluster_with_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

echo "Initial cluster time:\n";
var_dump($session->getClusterTime());

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);

echo "\nCluster time after command:\n";
var_dump($session->getClusterTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Initial cluster time:
NULL

Cluster time after command:
object(stdClass)#%d (%d) {
  ["clusterTime"]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(%d) "%d"
    ["timestamp"]=>
    string(%d) "%d"
  }
  ["signature"]=>
  %a
}
===DONE===

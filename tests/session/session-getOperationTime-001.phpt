--TEST--
MongoDB\Driver\Session::getOperationTime()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_sharded_cluster_with_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

echo "Initial operation time:\n";
var_dump($session->getOperationTime());

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);

echo "\nOperation time after command:\n";
var_dump($session->getOperationTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Initial operation time:
NULL

Operation time after command:
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(%d) "%d"
  ["timestamp"]=>
  string(%d) "%d"
}
===DONE===

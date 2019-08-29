--TEST--
MongoDB\Driver\Session::advanceClusterTime()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$sessionA = $manager->startSession();
$sessionB = $manager->startSession();

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command, ['session' => $sessionA]);

echo "Initial cluster time of session B:\n";
var_dump($sessionB->getClusterTime());

$sessionB->advanceClusterTime($sessionA->getClusterTime());

echo "\nCluster time after advancing session B:\n";
var_dump($sessionB->getClusterTime());

echo "\nSessions A and B have equivalent cluster times:\n";
var_dump($sessionA->getClusterTime() == $sessionB->getClusterTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Initial cluster time of session B:
NULL

Cluster time after advancing session B:
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

Sessions A and B have equivalent cluster times:
bool(true)
===DONE===

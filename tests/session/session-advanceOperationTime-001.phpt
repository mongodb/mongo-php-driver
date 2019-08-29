--TEST--
MongoDB\Driver\Session::advanceOperationTime()
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

echo "Initial operation time of session B:\n";
var_dump($sessionB->getOperationTime());

$sessionB->advanceOperationTime($sessionA->getOperationTime());

echo "\nOperation time after advancing session B:\n";
var_dump($sessionB->getOperationTime());

echo "\nSessions A and B have equivalent operation times:\n";
var_dump($sessionA->getOperationTime() == $sessionB->getOperationTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Initial operation time of session B:
NULL

Operation time after advancing session B:
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(%d) "%d"
  ["timestamp"]=>
  string(%d) "%d"
}

Sessions A and B have equivalent operation times:
bool(true)
===DONE===

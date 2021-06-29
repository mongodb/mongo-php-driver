--TEST--
MongoDB\Driver\Session debug output (after ending session)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);

$session->endSession();

var_dump($session);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Session)#%d (%d) {
  ["logicalSessionId"]=>
  NULL
  ["clusterTime"]=>
  NULL
  ["causalConsistency"]=>
  NULL
  ["operationTime"]=>
  NULL
  ["server"]=>
  NULL
  ["inTransaction"]=>
  NULL
  ["transactionState"]=>
  NULL
  ["transactionOptions"]=>
  NULL
}
===DONE===

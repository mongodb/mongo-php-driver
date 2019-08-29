--TEST--
MongoDB\Driver\Session debug output (after an operation)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$session = $manager->startSession();

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);

var_dump($session);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Session)#%d (%d) {
  ["logicalSessionId"]=>
  array(1) {
    ["id"]=>
    object(MongoDB\BSON\Binary)#%d (%d) {
      ["data"]=>
      string(16) "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
      ["type"]=>
      int(4)
    }
  }
  ["clusterTime"]=>
  array(2) {
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
  ["causalConsistency"]=>
  bool(true)
  ["operationTime"]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(%d) "%d"
    ["timestamp"]=>
    string(%d) "%d"
  }
  ["server"]=>
  NULL
}
===DONE===

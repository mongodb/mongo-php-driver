--TEST--
MongoDB\Driver\Session debug output (during a pinned transaction)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_mongos(); ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primary'));

$session = $manager->startSession();
$session->startTransaction();

$query = new MongoDB\Driver\Query([]);
$server->executeQuery(NS, $query, ['session' => $session]);

var_dump($session);

$session->abortTransaction();
$session->endSession();

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
      string(16) "%a"
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
  ["snapshot"]=>
  bool(false)
  ["operationTime"]=>
  object(MongoDB\BSON\Timestamp)#%d (%d) {
    ["increment"]=>
    string(%d) "%d"
    ["timestamp"]=>
    string(%d) "%d"
  }
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    %a
  }
  ["dirty"]=>
  bool(false)
  ["inTransaction"]=>
  bool(true)
  ["transactionState"]=>
  string(11) "in_progress"
  ["transactionOptions"]=>
  array(1) {
    ["readPreference"]=>
    object(MongoDB\Driver\ReadPreference)#%d (%d) {
      ["mode"]=>
      string(7) "primary"
    }
  }
}
===DONE===

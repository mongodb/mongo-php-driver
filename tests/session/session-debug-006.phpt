--TEST--
MongoDB\Driver\Session debug output (with transaction options)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_transactions(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

$options = [
    'maxCommitTimeMS' => 1,
    'readConcern' => new \MongoDB\Driver\ReadConcern('majority'),
    'readPreference' => new \MongoDB\Driver\ReadPreference('primaryPreferred'),
    'writeConcern' => new \MongoDB\Driver\WriteConcern('majority'),
];

$session->startTransaction($options);

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
      string(16) "%a"
      ["type"]=>
      int(4)
    }
  }
  ["clusterTime"]=>
  NULL
  ["causalConsistency"]=>
  bool(true)
  ["snapshot"]=>
  bool(false)
  ["operationTime"]=>
  NULL
  ["server"]=>
  NULL
  ["dirty"]=>
  bool(false)
  ["inTransaction"]=>
  bool(true)
  ["transactionState"]=>
  string(8) "starting"
  ["transactionOptions"]=>
  array(4) {
    ["maxCommitTimeMS"]=>
    int(1)
    ["readConcern"]=>
    object(MongoDB\Driver\ReadConcern)#%d (%d) {
      ["level"]=>
      string(8) "majority"
    }
    ["readPreference"]=>
    object(MongoDB\Driver\ReadPreference)#%d (%d) {
      ["mode"]=>
      string(16) "primaryPreferred"
    }
    ["writeConcern"]=>
    object(MongoDB\Driver\WriteConcern)#%d (%d) {
      ["w"]=>
      string(8) "majority"
    }
  }
}
===DONE===

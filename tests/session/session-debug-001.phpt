--TEST--
MongoDB\Driver\Session debug output (before an operation)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

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
  bool(false)
  ["transactionState"]=>
  string(4) "none"
  ["transactionOptions"]=>
  NULL
}
===DONE===

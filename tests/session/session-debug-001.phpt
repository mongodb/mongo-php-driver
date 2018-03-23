--TEST--
MongoDB\Driver\Session debug output (before an operation)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php NEEDS('STANDALONE'); NEEDS_ATLEAST_MONGODB_VERSION(STANDALONE, "3.6"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
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
      string(16) "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
      ["type"]=>
      int(4)
    }
  }
  ["clusterTime"]=>
  NULL
  ["causalConsistency"]=>
  bool(true)
  ["operationTime"]=>
  NULL
}
===DONE===

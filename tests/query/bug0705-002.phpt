--TEST--
PHPC-705: Do not unnecessarily wrap filters in $query (currentOp query)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE_30) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE_30);
$cursor = $manager->executeQuery('admin.$cmd.sys.inprog', new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(1) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["inprog"]=>
    array(0) {
    }
  }
}
===DONE===

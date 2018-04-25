--TEST--
PHPC-155: WriteConcernError->getInfo() can be scalar
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$wc = new MongoDB\Driver\WriteConcern("MultipleDC", 500);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('example' => 'document'));

try {
    $manager->executeBulkWrite(NS, $bulk, $wc);
} catch(MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult()->getWriteConcernError());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcernError)#%d (%d) {
  ["message"]=>
  string(%d) "%s"
  ["code"]=>
  int(79)
  ["info"]=>
  NULL
}
===DONE===

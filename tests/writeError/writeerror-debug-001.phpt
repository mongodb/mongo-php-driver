--TEST--
MongoDB\Driver\WriteError debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 1]);

try {
    $manager->executeBulkWrite(NS, $bulk);
} catch(MongoDB\Driver\Exception\BulkWriteException $e) {
    var_dump($e->getWriteResult()->getWriteErrors()[0]);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteError)#%d (%d) {
  ["message"]=>
  string(%d) "%SE11000 duplicate key error %s: phongo.writeError_writeerror_debug_001%s dup key: { %S: 1 }"
  ["code"]=>
  int(11000)
  ["index"]=>
  int(1)
  ["info"]=>
  NULL
}
===DONE===

--TEST--
MongoDB\Driver\Cursor handles invalid positions gracefully
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulkWrite = new MongoDB\Driver\BulkWrite;
$bulkWrite->insert(array('_id' => 0));

$writeResult = $manager->executeBulkWrite(NS, $bulkWrite);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));

$cursor->rewind();
var_dump($cursor->valid());
var_dump($cursor->key());
var_dump($cursor->current());

$cursor->next();
var_dump($cursor->valid());
var_dump($cursor->key());
var_dump($cursor->current());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
int(0)
object(stdClass)#%d (1) {
  ["_id"]=>
  int(0)
}
bool(false)
NULL
NULL
===DONE===

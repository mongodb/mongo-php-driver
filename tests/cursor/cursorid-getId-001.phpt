--TEST--
MongoDB\Driver\Cursor::getId
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([], ['batchSize' => 2]));

var_dump($cursor->getId());
var_dump($cursor->getId(false));
var_dump($cursor->getId(true));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Deprecated: MongoDB\Driver\Cursor::getId(): The method "MongoDB\Driver\Cursor::getId" will no longer return a "MongoDB\Driver\CursorId" instance in the future. Pass "true" as argument to change to the new behavior and receive a "MongoDB\BSON\Int64" instance instead. in %s
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  %rint\(%d\)|string\(%d\) "%d"%r
}

Deprecated: MongoDB\Driver\Cursor::getId(): The method "MongoDB\Driver\Cursor::getId" will no longer return a "MongoDB\Driver\CursorId" instance in the future. Pass "true" as argument to change to the new behavior and receive a "MongoDB\BSON\Int64" instance instead. in %s
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  %rint\(%d\)|string\(%d\) "%d"%r
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(%d) "%d"
}
===DONE===

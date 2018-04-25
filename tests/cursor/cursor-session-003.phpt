--TEST--
MongoDB\Driver\Cursor debug output for command cursor includes explicit session
--SKIPIF--
<?php require __DIR__ . "/" ."../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [['$match' => new stdClass]],
    'cursor' => ['batchSize' => 2],
]);
$session = $manager->startSession();

$cursor = $manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);

$iterator = new IteratorIterator($cursor);
$iterator->rewind();
$iterator->next();

printf("Cursor ID is zero: %s\n", (string) $cursor->getId() === '0' ? 'yes' : 'no');
var_dump($cursor);

$iterator->next();

/* Per PHPC-1161, the Cursor will free a reference to the Session as soon as it
 * is exhausted. While this is primarily done to ensure implicit sessions for
 * command cursors are returned to the pool ASAP, it also applies to explicit
 * sessions. */
printf("\nCursor ID is zero: %s\n", (string) $cursor->getId() === '0' ? 'yes' : 'no');
var_dump($cursor);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Cursor ID is zero: no
object(MongoDB\Driver\Cursor)#%d (%d) {
  %a
  ["session"]=>
  object(MongoDB\Driver\Session)#%d (%d) {
    %a
  }
  %a
}

Cursor ID is zero: yes
object(MongoDB\Driver\Cursor)#%d (%d) {
  %a
  ["session"]=>
  NULL
  %a
}
===DONE===

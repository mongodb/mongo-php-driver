--TEST--
MongoDB\Driver\Cursor debug output for command cursor includes implicit session
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

$cursor = $manager->executeCommand(DATABASE_NAME, $command);

$iterator = new IteratorIterator($cursor);
$iterator->rewind();
$iterator->next();

printf("Cursor ID is zero: %s\n", (string) $cursor->getId() === '0' ? 'yes' : 'no');
var_dump($cursor);

$iterator->next();

/* Unlike implicit sessions for query cursors, which are handled internally by
 * libmongoc, PHPC-1152 emulates its own implicit sessions for command cursors
 * in order to ensure that command cursors always share the same session as the
 * originating command. */
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

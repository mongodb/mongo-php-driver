--TEST--
MongoDB\Driver\Cursor debug output for query cursor omits implicit session
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

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);

$cursor = $manager->executeQuery(NS, $query);

$iterator = new IteratorIterator($cursor);
$iterator->rewind();
$iterator->next();

/* Implicit sessions for query cursors are never exposed to PHPC, as they are
 * handled internally by libmongoc. Cursor debug ouput should never report such
 * sessions. */
printf("Cursor ID is zero: %s\n", (string) $cursor->getId() === '0' ? 'yes' : 'no');
var_dump($cursor);

$iterator->next();

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
  NULL
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

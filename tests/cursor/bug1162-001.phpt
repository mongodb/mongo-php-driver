--TEST--
MongoDB\Driver\Cursor segfault dumping cursor while iterating with IteratorIterator
--SKIPIF--
<?php require __DIR__ . "/" ."../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
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
var_dump($cursor);

$iterator->next();
var_dump($cursor);

$iterator->next();
var_dump($cursor);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Cursor)#%d (%d) {%A
}
object(MongoDB\Driver\Cursor)#%d (%d) {%A
}
object(MongoDB\Driver\Cursor)#%d (%d) {%A
}
===DONE===

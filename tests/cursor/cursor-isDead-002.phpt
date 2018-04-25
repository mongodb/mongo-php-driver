--TEST--
MongoDB\Driver\Cursor::isDead() with IteratorIterator (find command)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([], ['batchSize' => 2]));

$iterator = new IteratorIterator($cursor);
$iterator->rewind();

for ($i = 0; $i < 3; $i++) {
    var_dump($cursor->isDead());
    $iterator->next();
}

var_dump($cursor->isDead());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(false)
bool(false)
bool(true)
===DONE===

--TEST--
MongoDB\Driver\Cursor::isDead()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, array('_id' => 1));
$manager->executeInsert(NS, array('_id' => 2));
$manager->executeInsert(NS, array('_id' => 3));

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array(), array('batchSize' => 2)));

foreach ($cursor as $_) {
    var_dump($cursor->isDead());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(false)
bool(false)
bool(true)
===DONE===

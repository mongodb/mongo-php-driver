--TEST--
MongoDB\Driver\WriteError::getInfo()
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
    // "errInfo" is rarely populated on a WriteError (e.g. shard version error)
    var_dump($e->getWriteResult()->getWriteErrors()[0]->getInfo());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
===DONE===

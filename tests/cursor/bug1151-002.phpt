--TEST--
PHPC-1151: Segfault if session unset before first getMore (aggregate)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
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
    'pipeline' => [],
    'cursor' => ['batchSize' => 2],
]);
$session = $manager->startSession();

$cursor = $manager->executeReadCommand(DATABASE_NAME, $command, ['session' => $session]);

foreach ($cursor as $document) {
    unset($session);
    echo $document->_id, "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
1
2
3
===DONE===

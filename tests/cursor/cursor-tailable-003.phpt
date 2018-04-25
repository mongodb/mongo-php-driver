--TEST--
MongoDB\Driver\Cursor tailable iteration with awaitData and maxAwaitTimeMS options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command([
    'create' => COLLECTION_NAME,
    'capped' => true,
    'size' => 1048576,
]));

$bulkWrite = new MongoDB\Driver\BulkWrite;
$bulkWrite->insert(['_id' => 1]);
$manager->executeBulkWrite(NS, $bulkWrite);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([], [
    'tailable' => true,
    'awaitData' => true,
    'maxAwaitTimeMS' => 10,
]));
$it = new IteratorIterator($cursor);

$it->rewind();
printf("{_id: %d}\n", $it->current()->_id);
$it->next();

$startTime = microtime(true);
echo "Awaiting results...\n";
$it->next();
printf("Waited for %.6f seconds\n", microtime(true) - $startTime);
// Sometimes the cursor will wait for 0.0099 seconds and sometimes it will wait for 0.01.
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
{_id: 1}
Awaiting results...
Waited for 0.0%d seconds
===DONE===

--TEST--
PHPC-1274: Session destruct should not abort transaction from parent process
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_no_transactions(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

/* Create collections as that can't be (automatically) done in a transaction */
$manager->executeCommand(
    DATABASE_NAME,
    new MongoDB\Driver\Command(['create' => COLLECTION_NAME]),
    ['writeConcern' => new MongoDB\Driver\WriteConcern('majority')]
);

$session = $manager->startSession();
$session->startTransaction(['writeConcern' => new MongoDB\Driver\WriteConcern('majority')]);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);
$result = $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);
printf("Parent inserted %d documents\n", $result->getInsertedCount());

$childPid = pcntl_fork();

if ($childPid === 0) {
    echo "Child exits\n";
    exit;
}

if ($childPid > 0) {
    $waitPid = pcntl_waitpid($childPid, $status);

    if ($waitPid === $childPid) {
        echo "Parent waited for child to exit\n";
    }

    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 3]);
    $bulk->insert(['x' => 4]);
    $result = $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);
    printf("Parent inserted %d documents\n", $result->getInsertedCount());

    $session->commitTransaction();

    $cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
    printf("Parent fully iterated cursor for %d documents\n", iterator_count($cursor));
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Parent inserted 2 documents
Child exits
Parent waited for child to exit
Parent inserted 2 documents
Parent fully iterated cursor for 4 documents
===DONE===

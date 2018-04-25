--TEST--
PHPC-913: Child process should not re-use mongoc_client_t objects from parent
--SKIPIF--
<?php if (!function_exists('pcntl_fork')) { die('skip pcntl_fork() not available'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php SLOW(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function logMyURI(MongoDB\Driver\Manager $manager)
{
    $command = new MongoDB\Driver\Command(['whatsmyuri' => 1]);
    $cursor = $manager->executeCommand(DATABASE_NAME, $command);
    $uri = $cursor->toArray()[0]->you;

    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['pid' => getmypid(), 'uri' => $uri]);
    $manager->executeBulkWrite(NS, $bulk);
}

$manager = new MongoDB\Driver\Manager(URI);
logMyURI($manager);

$parentPid = getmypid();
$childPid = pcntl_fork();

if ($childPid === 0) {
    $manager = new MongoDB\Driver\Manager(URI);
    logMyURI($manager);

    /* Due to PHPC-912, we cannot allow the child process to terminate before
     * the parent is done using its client, lest it destroy the mongoc_client_t
     * object and shutdown its socket(s). Sleep for 250ms to allow the parent
     * time to query for our logged URI. */
    usleep(250000);
    exit;
}

if ($childPid) {
    /* Sleep for 100ms to allow the child time to log its URI. Ideally, we would
     * wait for the child to finish, but PHPC-912 prevents us from doing so. */
    usleep(100000);

    $cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
    $results = $cursor->toArray();

    printf("%d connections were logged\n", count($results));
    printf("PIDs differ: %s\n", $results[0]->pid !== $results[1]->pid ? 'yes' : 'no');
    printf("URIs differ: %s\n", $results[0]->uri !== $results[1]->uri ? 'yes' : 'no');

    $waitPid = pcntl_waitpid($childPid, $status);

    if ($waitPid > 0) {
        printf("Parent(%d) waited for child(%d) to exit\n", $parentPid, $waitPid);
    }
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
2 connections were logged
PIDs differ: yes
URIs differ: yes
Parent(%d) waited for child(%d) to exit
===DONE===

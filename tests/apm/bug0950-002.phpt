--TEST--
PHPC-950: Segfault killing cursor after subscriber HashTable is destroyed (one subscriber)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        printf("- started: %s\n", $event->getCommandName());
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
        printf("- succeeded: %s\n", $event->getCommandName());
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
        printf("- failed: %s\n", $event->getCommandName());
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

MongoDB\Driver\Monitoring\addSubscriber(new MySubscriber);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([], ['batchSize' => 2]));

/* Exiting during iteration on a live cursor will result in
 * php_phongo_command_started() being invoked for the killCursor command after
 * RSHUTDOWN has already destroyed the subscriber HashTable */
foreach ($cursor as $data) {
    echo "Exiting during first iteration on cursor\n";
    exit(0);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
- started: find
- succeeded: find
Exiting during first iteration on cursor

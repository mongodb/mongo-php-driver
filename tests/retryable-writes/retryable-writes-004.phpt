--TEST--
Retryable writes: unacknowledged write operations do not include transaction IDs
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class TransactionIdObserver implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $command = $event->getCommand();
        $hasTransactionId = isset($command->lsid) && isset($command->txnNumber);

        printf("%s command includes transaction ID: %s\n", $event->getCommandName(), $hasTransactionId ? 'yes' : 'no');
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

$observer = new TransactionIdObserver;
MongoDB\Driver\Monitoring\addSubscriber($observer);

$manager = new MongoDB\Driver\Manager(URI);
$writeConcern = new MongoDB\Driver\WriteConcern(0);

echo "Testing unacknowledged deleteOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->delete(['x' => 1], ['limit' => 1]);
$manager->executeBulkWrite(NS, $bulk, ['writeConcern' => $writeConcern]);

echo "\nTesting unacknowledged insertOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$manager->executeBulkWrite(NS, $bulk, ['writeConcern' => $writeConcern]);

echo "\nTesting unacknowledged replaceOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['x' => 1], ['x' => 2]);
$manager->executeBulkWrite(NS, $bulk, ['writeConcern' => $writeConcern]);

echo "\nTesting unacknowledged updateOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]]);
$manager->executeBulkWrite(NS, $bulk, ['writeConcern' => $writeConcern]);

/* Note: the server does not actually support unacknowledged write concerns for
 * findAndModify. This is just testing that mongoc_cmd_parts_set_write_concern()
 * in libmongoc detects w:0 and refrains from adding a transaction ID. */
echo "\nTesting unacknowledged findAndModify\n";
$command = new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME,
    'query' => ['x' => 1],
    'update' => ['$inc' => ['x' => 1]],
]);
$manager->executeReadWriteCommand(DATABASE_NAME, $command, ['writeConcern' => $writeConcern]);

MongoDB\Driver\Monitoring\removeSubscriber($observer);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing unacknowledged deleteOne
delete command includes transaction ID: no

Testing unacknowledged insertOne
insert command includes transaction ID: no

Testing unacknowledged replaceOne
update command includes transaction ID: no

Testing unacknowledged updateOne
update command includes transaction ID: no

Testing unacknowledged findAndModify
findAndModify command includes transaction ID: no
===DONE===

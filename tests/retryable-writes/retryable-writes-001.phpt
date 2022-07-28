--TEST--
Retryable writes: supported single-statement operations include transaction IDs
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_sharded_cluster_with_replica_set(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class TransactionIdObserver implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event): void
    {
        $command = $event->getCommand();
        $hasTransactionId = isset($command->lsid) && isset($command->txnNumber);

        printf("%s command includes transaction ID: %s\n", $event->getCommandName(), $hasTransactionId ? 'yes' : 'no');
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event): void
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event): void
    {
    }
}

$observer = new TransactionIdObserver;
MongoDB\Driver\Monitoring\addSubscriber($observer);

$manager = create_test_manager();

echo "Testing deleteOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->delete(['x' => 1], ['limit' => 1]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting insertOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting replaceOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['x' => 1], ['x' => 2]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting updateOne\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting findAndModify\n";
$command = new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME,
    'query' => ['x' => 1],
    'update' => ['$inc' => ['x' => 1]],
]);
$manager->executeReadWriteCommand(DATABASE_NAME, $command);

MongoDB\Driver\Monitoring\removeSubscriber($observer);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing deleteOne
delete command includes transaction ID: yes

Testing insertOne
insert command includes transaction ID: yes

Testing replaceOne
update command includes transaction ID: yes

Testing updateOne
update command includes transaction ID: yes

Testing findAndModify
findAndModify command includes transaction ID: yes
===DONE===

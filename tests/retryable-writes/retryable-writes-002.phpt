--TEST--
Retryable writes: supported multi-statement operations include transaction IDs
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

echo "Testing multi-statement bulk write (ordered=true)\n";
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);
$bulk->delete(['x' => 1], ['limit' => 1]);
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]]);
$bulk->update(['x' => 1], ['x' => 2]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting multi-statement bulk write (ordered=false)\n";
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => false]);
$bulk->delete(['x' => 1], ['limit' => 1]);
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]]);
$bulk->update(['x' => 1], ['x' => 2]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting insertMany (ordered=true)\n";
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting insertMany (ordered=false)\n";
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => false]);
$bulk->insert(['x' => 1]);
$bulk->insert(['x' => 2]);
$manager->executeBulkWrite(NS, $bulk);

MongoDB\Driver\Monitoring\removeSubscriber($observer);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing multi-statement bulk write (ordered=true)
delete command includes transaction ID: yes
insert command includes transaction ID: yes
update command includes transaction ID: yes

Testing multi-statement bulk write (ordered=false)
delete command includes transaction ID: yes
insert command includes transaction ID: yes
update command includes transaction ID: yes

Testing insertMany (ordered=true)
insert command includes transaction ID: yes

Testing insertMany (ordered=false)
insert command includes transaction ID: yes
===DONE===

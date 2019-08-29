--TEST--
Retryable writes: unsupported operations do not include transaction IDs
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

echo "Testing deleteMany\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->delete(['x' => 1], ['limit' => 0]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting updateMany\n";
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]], ['multi' => true]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting multi-statement bulk write with one unsupported operation (ordered=true)\n";
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);
$bulk->delete(['x' => 1], ['limit' => 1]);
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]]);
$bulk->update(['x' => 1], ['x' => 2]);
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]], ['multi' => true]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting multi-statement bulk write with one unsupported operation (ordered=false)\n";
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => false]);
$bulk->delete(['x' => 1], ['limit' => 1]);
$bulk->insert(['x' => 1]);
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]]);
$bulk->update(['x' => 1], ['x' => 2]);
$bulk->update(['x' => 1], ['$inc' => ['x' => 1]], ['multi' => true]);
$manager->executeBulkWrite(NS, $bulk);

echo "\nTesting aggregate\n";
$command = new MongoDB\Driver\Command([
    'aggregate' => COLLECTION_NAME,
    'pipeline' => [
        ['$match' => ['x' => 1]],
        ['$out' => COLLECTION_NAME . '.out'],
    ],
    'cursor' => new stdClass,
]);
$manager->executeReadWriteCommand(DATABASE_NAME, $command);

MongoDB\Driver\Monitoring\removeSubscriber($observer);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing deleteMany
delete command includes transaction ID: no

Testing updateMany
update command includes transaction ID: no

Testing multi-statement bulk write with one unsupported operation (ordered=true)
delete command includes transaction ID: yes
insert command includes transaction ID: yes
update command includes transaction ID: no

Testing multi-statement bulk write with one unsupported operation (ordered=false)
delete command includes transaction ID: yes
insert command includes transaction ID: yes
update command includes transaction ID: no

Testing aggregate
aggregate command includes transaction ID: no
===DONE===

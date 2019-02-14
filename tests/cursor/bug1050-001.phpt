--TEST--
PHPC-1050: Command cursor should not invoke getMore at execution
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_server_storage_engine('wiredTiger'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$cmd = new MongoDB\Driver\Command(
    [
        'aggregate' => COLLECTION_NAME,
        'pipeline' => [
            ['$changeStream' => (object) []],
        ],
        'cursor' => (object) [],
    ],
    [
        'maxAwaitTimeMS' => 500,
    ]
);

$start = microtime(true);
$cursor = $manager->executeReadCommand(DATABASE_NAME, $cmd);
printf("Executing command took %0.6f seconds\n", microtime(true) - $start);

$it = new IteratorIterator($cursor);

$start = microtime(true);
$it->rewind();
printf("Rewinding cursor took %0.6f seconds\n", microtime(true) - $start);
printf("Current position is valid: %s\n", $it->valid() ? 'yes' : 'no');

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$manager->executeBulkWrite(NS, $bulk);

$start = microtime(true);
$it->next();
printf("Advancing cursor took %0.6f seconds\n", microtime(true) - $start);
printf("Current position is valid: %s\n", $it->valid() ? 'yes' : 'no');

$document = $it->current();

if (isset($document)) {
    printf("Operation type: %s\n", $document->operationType);
    var_dump($document->fullDocument);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Executing command took 0.%d seconds
Rewinding cursor took 0.%r(4|5)%r%d seconds
Current position is valid: no
Advancing cursor took %d.%d seconds
Current position is valid: yes
Operation type: insert
object(stdClass)#%d (%d) {
  ["_id"]=>
  object(MongoDB\BSON\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%x"
  }
  ["x"]=>
  int(1)
}
===DONE===

--TEST--
MongoDB\Driver\Manager with disabled client persistence outlives cursors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

ini_set('mongodb.debug', 'stderr');
$manager = new MongoDB\Driver\Manager(URI, [], ['disableClientPersistence' => true]);
ini_set('mongodb.debug', '');

echo "Inserting data\n";

// load fixtures for test
$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 2, 'y' => 3));
$bulk->insert(array('_id' => 2, 'x' => 3, 'y' => 4));
$bulk->insert(array('_id' => 3, 'x' => 4, 'y' => 5));
$manager->executeBulkWrite(NS, $bulk);

echo "Fetching cursor\n";

$query = new MongoDB\Driver\Query([], ['batchSize' => 1]);
$cursor = $manager->executeQuery(NS, $query);

echo "Destroying manager\n";

ini_set('mongodb.debug', 'stderr');
unset($manager);
ini_set('mongodb.debug', '');

echo "Reading data\n";

var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client with hash: %s
Inserting data
Fetching cursor
Destroying manager
Reading data
[%s]     mongoc: DEBUG   > Destroying non-persistent client for Manager%A
===DONE===

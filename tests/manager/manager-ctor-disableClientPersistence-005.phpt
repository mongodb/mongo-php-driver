--TEST--
MongoDB\Driver\Manager with disableClientPersistence=true referenced by WriteResult
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

ini_set('mongodb.debug', 'stderr');
$manager = create_test_manager(URI, [], ['disableClientPersistence' => true]);
ini_set('mongodb.debug', '');

echo "Inserting data\n";
$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1, 'x' => 2, 'y' => 3]);
$bulk->insert(['_id' => 2, 'x' => 3, 'y' => 4]);
$bulk->insert(['_id' => 3, 'x' => 4, 'y' => 5]);
$writeResult = $manager->executeBulkWrite(NS, $bulk);

echo "Unsetting manager\n";
ini_set('mongodb.debug', 'stderr');
unset($manager);
ini_set('mongodb.debug', '');

echo "Unsetting writeResult\n";
ini_set('mongodb.debug', 'stderr');
unset($writeResult);
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client
Inserting data
Unsetting manager
Unsetting writeResult%A
[%s]     PHONGO: DEBUG   > Destroying non-persistent client for Manager%A
===DONE===

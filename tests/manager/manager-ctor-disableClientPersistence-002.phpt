--TEST--
MongoDB\Driver\Manager with disableClientPersistence=true referenced by Cursor
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
$manager->executeBulkWrite(NS, $bulk);

echo "Creating cursor\n";
$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$cursor = $manager->executeQuery(NS, $query);

echo "Unsetting manager\n";
ini_set('mongodb.debug', 'stderr');
unset($manager);
ini_set('mongodb.debug', '');

echo "Iterating cursor\n";
var_dump(iterator_to_array($cursor));

echo "Unsetting cursor\n";
ini_set('mongodb.debug', 'stderr');
unset($cursor);
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client
Inserting data
Creating cursor
Unsetting manager
Iterating cursor
array(3) {
  [0]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(2)
    ["y"]=>
    int(3)
  }
  [1]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(3)
    ["y"]=>
    int(4)
  }
  [2]=>
  object(stdClass)#%d (3) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(4)
    ["y"]=>
    int(5)
  }
}
Unsetting cursor%A
[%s]     PHONGO: DEBUG   > Destroying non-persistent client for Manager%A
===DONE===

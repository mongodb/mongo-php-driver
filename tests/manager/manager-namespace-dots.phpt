--TEST--
MongoDB\Driver\Manager: a period in the collection name stays valid
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

/* A period separates the database and the collection name at the first
 * occurrence, so it stays valid inside a collection name. */
$collectionName = COLLECTION_NAME . '.with.dots';
$namespace = DATABASE_NAME . '.' . $collectionName;

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$result = $manager->executeBulkWrite($namespace, $bulk);
printf("insertedCount: %d\n", $result->getInsertedCount());

/* listCollections confirms the collection was created with the full name,
 * including the periods, instead of splitting the namespace. */
$cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command([
    'listCollections' => 1,
    'filter' => ['name' => $collectionName],
]));
$collections = $cursor->toArray();
printf("matched collections: %d\n", count($collections));
printf("name matches: %s\n", var_export($collections[0]->name === $collectionName, true));

$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['drop' => $collectionName]));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
insertedCount: 1
matched collections: 1
name matches: true
===DONE===

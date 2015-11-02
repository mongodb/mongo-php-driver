# Bulk CRUD Operations

```php
<?php

$hannes = [
    "name"    => "Hannes",
    "nick"    => "bjori",
    "citizen" => "Iceland",
];
$hayley = [
    "name"    => "Hayley",
    "nick"    => "Alien Ninja",
    "citizen" => "USA",
];
$jonpall = [
    "name"    => "Jon Pall",
    "nick"    => "unknown",
    "citizen" => "Iceland",
];

/* Ordered bulk operations (default) are executed in the same order that we add
 * the write operations. If an operation fails, execution stops and no further
 * operations are attempted. For unordered bulk operations, the operations may
 * be executed in any order by the database in an attempt to optimize its
 * workload. An operation failure within an unordered batch will not stop
 * execution of additional write operations in that batch. */
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);

?>
```

## Create

```php
<?php

/* Specify the document to insert (array or object) as the first argument. If a
 * document does not already have an "_id" field, a generated
 * MongoDB\BSON\ObjectId will be returned. */
$hannes_id  = $bulk->insert($hannes);
$hayley_id  = $bulk->insert($hayley);
$jonpall_id = $bulk->insert($jonpall);

?>
```

## Update

```php
<?php

/* Arguments:
 *
 *  #1 (array|object): search criteria to select the document(s) for updating
 *  #2 (array|object): replacement document or atomic operations to apply
 *  #3 (array): update options
 *    * limit (integer): Updates all matching documents when 0 (false).
 *      Otherwise, only the first matching document is updated.
 *    * upsert (boolean): If there is no matching document, create a new
 *      document from $filter and $update. */
$bulk->update(
    ["_id" => $hayley_id],
    ['$set' => ["citizen" => "Iceland"]],
    ["limit" => 1, "upsert" => false]
);
$bulk->update(
    ["citizen" => "Iceland"],
    ['$set' => ["viking" => true]],
    ["limit" => 0, "upsert" => false]
);
$bulk->update(
    ["name" => "Chuck Norris"],
    ['$set' => ["viking" => false]],
    ["limit" => 1, "upsert" => true]
];

?>
```

## Delete

```php
<?php

/* Arguments:
 *
 *  #1 (array|object): search criteria to select the document(s) for deleting
 *  #3 (array): delete options
 *    * limit (integer): Deletes all matching documents when 0 (false).
 *      Otherwise, only the first matching document is deleted. */
$bulk->delete(
    ["_id" => $jonpall_id],
    ["limit" => 1]
);

?>
```

## Executing and checking the results

```php
<?php

// Construct a write concern
$wc = new MongoDB\Driver\WriteConcern(
    // Guarantee that writes are acknowledged by a majority of our nodes
    MongoDB\Driver\WriteConcern::MAJORITY,
    // But only wait 1000ms because we have an application to run!
    1000
);

$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");
$result  = $manager->executeBulkWrite("db.collection", $bulk, $wc);

printf("insertedCount: %d\n", $result->getInsertedCount());
printf("matchedCount:  %d\n", $result->getMatchedCount());
printf("modifiedCount: %d\n", $result->getModifiedCount());
printf("upsertedCount: %d\n", $result->getUpsertedCount());
printf("deletedCount:  %d\n", $result->getDeletedCount());

foreach ($result->getUpsertedIds() as $index => $id) {
    printf("upsertedId: '%s', index: %d\n", $id, $index);
}

$query  = new MongoDB\Driver\Query(["viking" => false]);
$cursor = $manager->executeQuery("db.collection", $query);

/* Note that var_dump()-ing the $cursor directly will print out additional debug
 * information about the cursor, such as the read preferenced used, the query
 * executed, namespace, query flags, and information about the current batch of
 * results from the server. Instead, we'll simply var_dump() the results. */
var_dump($cursor->toArray());

?>
```

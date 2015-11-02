# Basic CRUD Operations

## Setup and connecting

```php
<?php

// The document that we'll be working with
$document = ["hello" => "world"];

// Construct a write concern
$wc = new MongoDB\Driver\WriteConcern(
    // Guarantee that writes are acknowledged by a majority of our nodes
    MongoDB\Driver\WriteConcern::MAJORITY,
    // But only wait 1000ms because we have an application to run!
    1000
);

// Construct a read preference
$rp = new MongoDB\Driver\ReadPreference(
    /* We prefer to read from a secondary, but are OK with reading from the
     * primary if necessary (e.g. secondaries are offline) */
    MongoDB\Driver\ReadPreference::RP_SECONDARY_PREFERRED,
    // Specify some tag sets for our preferred nodes
    [
        // Prefer reading from our west coast datacenter in Iceland
        ["country" => "iceland", "datacenter" => "west"],
        // Fall back to any datacenter in Iceland
        ["country" => "iceland"],
        // If Iceland is offline, read from whatever is available
        [],
    ]
);

/* Construct the MongoDB Manager. For brevity, we're only connecting to one node
 * here, but you'd likely want to specify several replica set members in a
 * comma-delimited seed list */
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");

?>
```

## Create

```php
<?php

// Create a bulk write object and add our insert operation
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert($document);

try {
    /* Specify the full namespace as the first argument, followed by the bulk
     * write object and an optional write concern. MongoDB\Driver\WriteResult is
     * returned on success; otherwise, an exception is thrown. */
    $result = $manager->executeBulkWrite("db.collection", $bulk, $wc);
    var_dump($result);
} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
}

?>
```

## Read

```php
<?php

/* Construct a query with an empty filter (i.e. "select all") */
$query = new MongoDB\Driver\Query([]);

try {
    /* Specify the full namespace as the first argument, followed by the query
     * object and an optional read preference. MongoDB\Driver\Cursor is returned
     * success; otherwise, an exception is thrown. */
    $cursor = $manager->executeQuery("db.collection", $query, $rp);

    // Iterate over all matched documents
    foreach ($cursor as $document) {
        var_dump($document);
    }
} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
}

?>
```

## Update

```php
<?php

// Specify the search criteria and update operations (or replacement document)
$filter = ["hello" => "world"];
$update = ['$set' => ["hello" => "wonderful world"]];

/* Specify some command options for the update:
 *
 *  * limit (integer): Updates all matching documents when 0 (false). Otherwise,
 *    only the first matching document is updated.
 *  * upsert (boolean): If there is no matching document, create a new document
 *    from $filter and $update. */
$options = ["limit" => 1, "upsert" => false];

// Create a bulk write object and add our update operation
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update($filter, $update, $options);

try {
    /* Specify the full namespace as the first argument, followed by the bulk
     * write object and an optional write concern. MongoDB\Driver\WriteResult is
     * returned on success; otherwise, an exception is thrown. */
    $result = $manager->executeBulkWrite("db.collection", $bulk, $wc);
    var_dump($result);
} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
}

?>
```

## Delete

```php
<?php

// Specify the search criteria
$filter = ["hello" => "world"];

/* Specify some command options for the update:
 *
 *  * limit (integer): Deletes all matching documents when 0 (false). Otherwise,
 *    only the first matching document is deleted. */
$options = ["limit" => 1];

// Create a bulk write object and add our delete operation
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->delete($filter, $options);

try {
    /* Specify the full namespace as the first argument, followed by the bulk
     * write object and an optional write concern. MongoDB\Driver\WriteResult is
     * returned on success; otherwise, an exception is thrown. */
    $result = $manager->executeBulkWrite("db.collection", $bulk, $wc);
    var_dump($result);
} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
}

?>
```

# Basic CRUD

```php
<?php

/* The document we'll be working with */
$document = array("hello" => "world");

/* We want to gurantee all writes to the majority of our nodes */
$w = MongoDB\WriteConcern::MAJORITY;

/* But don't be waiting on me fore more then 1000ms (1sec),
 * I have an application to run! */
$wtimeout = 1000;

/* No need to journal or fsync (are infact discouraged in general) */
$journal = $fsync = false;

/* Construct the WriteConcern object from our options */
$wc = new MongoDB\WriteConcern($w, $wtimeout, $journal, $fsync);


/* We prefer to read from the secondary, but are OK to read from the primary
 * if we can't find any secondaries */
$prefer = MongoDB\ReadPreference::RP_SECONDARY_PREFERRED;
$tags = array(
	/* Prefer the West Coast datacenter in Iceland */
	array("country" => "iceland", "datacenter" => "west"),

	/* Fallback to any datacenter in Iceland */
	array("country" => "iceland"),

	/* If Iceland is offline, read from whatever is online! */
	array(),
);

/* Construct the ReadPreference object from our options */
$rp = new MongoDB\ReadPreference($prefer, $tags);


/* Construct the MongoDB Manager */
$manager = new MongoDB\Manager("mongodb://localhost:27017");

?>
```

## CREATE

```php
<?php

try {
	/* Full namespace as the first argumnet (databasename.collectionname),
	 * and the document to insert as the second.
	 *
	 * Returns MongoDB\WriteResult on success, throws exception on failure
	 */
	$result = $manager->executeInsert("db.collection", $document, $wc);
} catch(Exception $e) {
	echo $e->getMessage(), "\n";
	exit;
}

?>
```

## READ

```php
<?php

/* Construct an empty find query ("select all") */
$query = new MongoDB\Query(array());

try {
	/* Full namespace as the first argument (dbname.collname), and the query object
	 * to execute as the second.
	 * Returns MongoDB\QueryResult on success, throws exception on failure
	 */
	$cursor = $manager->executeQuery("db.collection", $query, $rp);

	/* Iterate over all matched documents */
	foreach($cursor as $document) {
		var_dump($document);
	}
} catch(Exception $e) {
	echo $e->getMessage(), "\n";
	exit;
}

?>
```

## UPDATE

```php
<?php

/* The search criteria */
$where = array("hello" => "world");

/* What to update the matched document with */
$set = array('$set' => array("hello" => "wonderful world"));

/* Special MongoDB wireprotocol options:
 *   limit: integer. Updates all matching document when 0 (false).
 *                   Only the first matching document when > 0 (true).
 *   upsert: boolean. If there is no matching document, create a new document
 *                    from the $where and apply the $set
 */
$options = array("limit" => 1, "upsert" => false);
try {
	/* Full namespace as the first argument (dbname.collname), the search criteria
	 * second.
	 * Returns MongoDB\WriteResult on success, throws exception on failure
	 */
	$result = $manager->executeUpdate("db.collection", $where, $set, $options, $wc);
	var_dump($result);
} catch(Exception $e) {
	echo $e->getMessage(), "\n";
	exit;
}

?>
```

## DELETE

```php
<?php

/* The search criteria */
$where = array("hello" => "world");

/* Special MongoDB wireprotocol options:
 *   limit: integer. Updates all matching document when > 0 (true).
 *                   Only the first matching document when 0 (false).
 */
$options = array("limit" => 1);
try {
	/*
	/* Full namespace as the first argument (dbname.collname), the search criteria
	 * second.
	 * Returns MongoDB\WriteResult on success, throws exception on failure
	 */
	$result = $manager->executeDelete("db.collection", $where, $options, $wc);
	var_dump($result);
} catch(Exception $e) {
	echo $e->getMessage(), "\n";
	exit;
}

?>
```



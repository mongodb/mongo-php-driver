# Executing Commands

# Listing databases and collections

```php
<?php

// Construct the MongoDB Manager
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");

// Construct and execute the listDatabases command
$listdatabases = new MongoDB\Driver\Command(["listDatabases" => 1]);
$result        = $manager->executeCommand("admin", $listdatabases);

/* The command returns a single result document, which contains the information
 * for all databases in a "databases" array field. */
$databases     = current($result->toArray());

foreach ($databases["databases"] as $database) {
	echo $database->name, "\n";

	// Construct and execute the listCollections command for each database
	$listcollections = new MongoDB\Driver\Command(["listCollections" => 1]);
	$result          = $manager->executeCommand($database->name, $listcollections);

	/* The command returns a cursor, which we can iterate on to access
	 * information for each collection. */
	$collections     = $result->toArray();

	foreach ($collections as $collection) {
		echo "\t * ", $collection->name, "\n";
	}
}

?>
```

# Create a user

```php
<?php

// Construct the MongoDB Manager
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");

// Construct the createUser command
$createuser = new MongoDB\Driver\Command([
	"createUser" => "USERNAME2",
	"pwd"        => "PASSWORD",
	"roles"      => [
		["role" => "clusterAdmin",         "db" => "admin"],
		["role" => "readWriteAnyDatabase", "db" => "admin"],
		["role" => "userAdminAnyDatabase", "db" => "admin"],
		"readWrite",
	],
	"writeConcern" => ["w" => "majority"],
]);

try {
	$result     = $manager->executeCommand("admin", $createuser);
	$response   = current($result->toArray());

	if ($response["ok"]) {
		echo "User created successfully\n";
	}
} catch (MongoDB\Driver\Exception\Exception $e) {
	echo $e->getMessage(), "\n";
}

?>
```

## Commands and read preferences

```php
<?php

/* Some commands (e.g. count, aggregate) may be executed on secondaries. As with
 * normal queries, we can supply a MongoDB\Driver\ReadPreference object when
 * executing a command in order to prefer certain servers over others. */

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

// Construct the MongoDB Manager
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");

$query = ["citizen" => "Iceland"];
$count = new MongoDB\Driver\Command(["count" => "collection", "query" => $query]);

try {
	$result   = $manager->executeCommand("db", $count, $rp);
	$response = current($result->toArray());

	if ($response["ok"]) {
		printf("db.collection has %d document(s) matching: %s\n",
			$response["n"],
			MongoDB\BSON\toJSON(MongoDB\BSON\fromArray($query))
		);
	}
} catch (MongoDB\Driver\Exception\Exception $e) {
	echo $e->getMessage(), "\n";
}

?>
```

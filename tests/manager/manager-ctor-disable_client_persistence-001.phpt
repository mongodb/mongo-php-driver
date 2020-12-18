--TEST--
MongoDB\Driver\Manager::__construct() allows disabling client persistence
--FILE--
<?php

ini_set('mongodb.debug', 'stderr');
new MongoDB\Driver\Manager();

// Won't reuse first client instance due to different options
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => false]);
// Will reuse the previously created client
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => false]);

// Will create a new private client
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => true]);

// Will create another new private client
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => true]);
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Persisted client with hash: %s
[%s]     mongoc: DEBUG   > Not destroying persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Persisted client with hash: %s
[%s]     mongoc: DEBUG   > Not destroying persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Found client for hash: %s
[%s]     mongoc: DEBUG   > Not destroying persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     mongoc: DEBUG   > Destroying non-persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     mongoc: DEBUG   > Destroying non-persistent client for Manager%A
===DONE===

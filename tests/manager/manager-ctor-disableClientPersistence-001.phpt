--TEST--
MongoDB\Driver\Manager::__construct(): disableClientPersistence option
--FILE--
<?php

ini_set('mongodb.debug', 'stderr');
new MongoDB\Driver\Manager();

// Won't reuse first client due to different options
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => false]);

// Will reuse the previous client due to same options
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => false]);

// Will create a non-persistent client
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => true]);

// Will create another non-persistent client
new MongoDB\Driver\Manager(null, [], ['disableClientPersistence' => true]);
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored persistent client with hash: %s
[%s]     PHONGO: DEBUG   > Not destroying persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored persistent client with hash: %s
[%s]     PHONGO: DEBUG   > Not destroying persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Found client for hash: %s
[%s]     PHONGO: DEBUG   > Not destroying persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client
[%s]     PHONGO: DEBUG   > Destroying non-persistent client for Manager%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client
[%s]     PHONGO: DEBUG   > Destroying non-persistent client for Manager%A
===DONE===

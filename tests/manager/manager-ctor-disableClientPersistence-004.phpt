--TEST--
MongoDB\Driver\Manager with disableClientPersistence=true referenced by Server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

ini_set('mongodb.debug', 'stderr');
$manager = create_test_manager(URI, [], ['disableClientPersistence' => true]);
ini_set('mongodb.debug', '');

echo "Creating server\n";
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('nearest'));

echo "Unsetting manager\n";
ini_set('mongodb.debug', 'stderr');
unset($manager);
ini_set('mongodb.debug', '');

echo "Unsetting server\n";
ini_set('mongodb.debug', 'stderr');
unset($server);
ini_set('mongodb.debug', '');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%A
[%s]     PHONGO: DEBUG   > Created client with hash: %s
[%s]     PHONGO: DEBUG   > Stored non-persistent client
Creating server
Unsetting manager
Unsetting server%A
[%s]     PHONGO: DEBUG   > Destroying non-persistent client for Manager%A
===DONE===

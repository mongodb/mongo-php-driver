--TEST--
PHPC-655: Use case insensitive parsing for Manager connectTimeoutMS array option
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM does not use custom streams"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$command = new MongoDB\Driver\Command(['ping' => 1]);

// Invalid host cannot be resolved
$manager = new MongoDB\Driver\Manager('mongodb://invalid.host:27017', ['connectTimeoutMS' => 1]);

ini_set('mongodb.debug', 'stderr');

echo throws(function() use ($manager, $command) {
    $manager->executeCommand(DATABASE_NAME, $command);
}, 'MongoDB\Driver\Exception\ConnectionTimeoutException'), "\n";

// Valid host refuses connection
$manager = new MongoDB\Driver\Manager('mongodb://localhost:54321', ['CONNECTTIMEOUTMS' => 1]);

echo throws(function() use ($manager, $command) {
    $manager->executeCommand(DATABASE_NAME, $command);
}, 'MongoDB\Driver\Exception\ConnectionTimeoutException'), "\n";

ini_set('mongodb.debug', 'off');

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
[%s]     PHONGO: DEBUG   > Applying connectTimeoutMS: 1
[%s]     PHONGO: DEBUG   > Connecting to 'invalid.host:27017[mongodb://invalid.host:27017]'
%a
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverselectiontryonce` set): %s
%a
[%s]     PHONGO: DEBUG   > Applying connectTimeoutMS: 1
[%s]     PHONGO: DEBUG   > Connecting to 'localhost:54321[mongodb://localhost:54321]'
%a
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverselectiontryonce` set): %s
===DONE===

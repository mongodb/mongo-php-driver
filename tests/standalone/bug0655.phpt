--TEST--
PHPC-655: Use case insensitive parsing for Manager connectTimeoutMS array option
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$command = new MongoDB\Driver\Command(['ping' => 1]);

// Invalid host cannot be resolved
$manager = create_test_manager('mongodb://example.invalid:27017', ['connectTimeoutMS' => 1]);

echo throws(function() use ($manager, $command) {
    $manager->executeCommand(DATABASE_NAME, $command);
}, 'MongoDB\Driver\Exception\ConnectionTimeoutException'), "\n";

// Valid host refuses connection
$manager = create_test_manager('mongodb://localhost:54321', ['CONNECTTIMEOUTMS' => 1]);

echo throws(function() use ($manager, $command) {
    $manager->executeCommand(DATABASE_NAME, $command);
}, 'MongoDB\Driver\Exception\ConnectionTimeoutException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverSelectionTryOnce` set): %s
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverSelectionTryOnce` set): %s
===DONE===

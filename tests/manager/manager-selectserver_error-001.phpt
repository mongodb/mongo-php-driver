--TEST--
MongoDB\Driver\Manager::selectServer() should not issue warning before exception
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY);

// Invalid host cannot be resolved
$manager = create_test_manager('mongodb://example.invalid:27017', ['serverSelectionTimeoutMS' => 1]);

echo throws(function() use ($manager, $rp) {
    $manager->selectServer($rp);
}, 'MongoDB\Driver\Exception\ConnectionTimeoutException'), "\n";

// Valid host refuses connection
$manager = create_test_manager('mongodb://localhost:54321', ['serverSelectionTimeoutMS' => 1]);

echo throws(function() use ($manager, $rp) {
    $manager->selectServer($rp);
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

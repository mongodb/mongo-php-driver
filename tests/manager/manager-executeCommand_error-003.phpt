--TEST--
MongoDB\Driver\Manager::executeCommand() connection error
--FILE--
<?php
require_once __DIR__ . "/../utils/tools.php";

$manager = new MongoDB\Driver\Manager("mongodb://localhost:44444/?serverselectiontimeoutms=10");

$command = new MongoDB\Driver\Command(['ping' => 1]);

echo throws(function() use($manager, $command) {
    $manager->executeCommand('test', $command);
}, "MongoDB\Driver\Exception\ConnectionTimeoutException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverSelectionTryOnce` set): %s
===DONE===

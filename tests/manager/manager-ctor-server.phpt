--TEST--
MongoDB\Driver\Manager::__construct(): check if server is live
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new \MongoDB\Driver\Manager(URI);

$command = new MongoDB\Driver\Command(['ping' => 1]);

try {
    $manager->executeCommand("test", $command);
} catch (\MongoDB\Driver\Exception\ConnectionException $e) {
    echo "Could not connect to MongoDB server: ", $e->getMessage(), "\n";
}
?>
===DONE===
--EXPECT--
===DONE===

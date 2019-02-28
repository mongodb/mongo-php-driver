--TEST--
MongoDB\Driver\Manager::__construct(): wire version support
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new \MongoDB\Driver\Manager(URI);

$command = new MongoDB\Driver\Command(['ping' => 1]);
try {
    $manager->executeCommand("test", $command);
} catch (\MongoDB\Driver\Exception\ConnectionException $e) {
    if ($e->getCode() == 15) { // MONGOC_ERROR_PROTOCOL_BAD_WIRE_VERSION
        echo "Bad wire version detected: ", $e->getMessage(), "\n";
    }
}
?>
===DONE===
--EXPECT--
===DONE===

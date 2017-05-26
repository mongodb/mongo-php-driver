--TEST--
Test connection to MongoDB.
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$adminmanager = new MongoDB\Driver\Manager("mongodb://localhosta:27017");

try {
    $adminmanager->pingConnection();
    echo "Connection Established\n";
} catch (\MongoDB\Driver\Exception\ConnectionException $e) {
    echo $e->getMessage() . "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Failed to create Manager from URI
===DONE===


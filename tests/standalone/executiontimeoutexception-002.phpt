--TEST--
ExecutionTimeoutException: exceeding maxTimeMS (commands)
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI);

$cmd = array(
    "count" => "collection",
    "query" => array("a" => 1),
    "maxTimeMS" => 100,
);
$command = new MongoDB\Driver\Command($cmd);

failMaxTimeMS($manager);
throws(function() use ($manager, $command) {
    $result = $manager->executeCommand(DATABASE_NAME, $command);
}, "MongoDB\Driver\ExecutionTimeoutException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\ExecutionTimeoutException
===DONE===

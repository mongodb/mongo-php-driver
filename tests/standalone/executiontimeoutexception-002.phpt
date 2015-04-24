--TEST--
ExecutionTimeoutException: exceeding maxTimeMS (commands)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
<?php TESTCOMMANDS(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$cmd = array(
    "count" => "collection",
    "query" => array("a" => 1),
    "maxTimeMS" => 100,
);
$command = new MongoDB\Driver\Command($cmd);

failMaxTimeMS($manager);
throws(function() use ($manager, $command) {
    $result = $manager->executeCommand(DATABASE_NAME, $command);
}, "MongoDB\Driver\Exception\ExecutionTimeoutException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\ExecutionTimeoutException
===DONE===

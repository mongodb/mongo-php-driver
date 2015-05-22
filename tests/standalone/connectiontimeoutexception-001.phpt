--TEST--
ConnectionTimeoutException: exceeding sockettimeoutms
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
<?php TESTCOMMANDS(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/" . "../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE . "/?sockettimeoutms=9");

$cmd = array(
    "sleep" => 1,
    "w" => false,
    "secs" => 2,
);
$command = new MongoDB\Driver\Command($cmd);

throws(function() use ($manager, $command) {
    $result = $manager->executeCommand("admin", $command);
    var_dump($result->toArray());
}, "MongoDB\Driver\Exception\\ConnectionTimeoutException");

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
===DONE===

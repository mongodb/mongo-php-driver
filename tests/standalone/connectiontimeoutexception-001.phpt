--TEST--
ConnectionTimeoutException: exceeding sockettimeoutms
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(MONGODB_URI . "/?sockettimeoutms=1004");

$cmd = array(
    "sleep" => 1,
    "w" => false,
    "secs" => 2,
);
$command = new MongoDB\Driver\Command($cmd);

throws(function() use ($manager, $command) {
    $result = $manager->executeCommand("admin", $command);
}, "MongoDB\Driver\ConnectionTimeoutException");

/* Sleep one second longer then needing to make sure later processes don't get affected */
sleep(3);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\ConnectionTimeoutException
===DONE===

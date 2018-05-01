--TEST--
ConnectionTimeoutException: exceeding sockettimeoutms
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_test_commands_disabled(); ?>
--FILE--
<?php
require_once __DIR__ . "/" . "../utils/basic.inc";

$uri = append_uri_option(URI, 'sockettimeoutms=9');
$manager = new MongoDB\Driver\Manager($uri);

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

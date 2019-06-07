--TEST--
ConnectionTimeoutException: exceeding sockettimeoutms
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_test_commands_disabled(); ?>
<?php skip_if_sleep_command_unavailable(); ?>
--FILE--
<?php
require_once __DIR__ . "/" . "../utils/basic.inc";

/* A 500ms socket timeout will ensure that SDAM and any authentication can
 * complete before executing a sleep command to trigger a network error. */
$uri = append_uri_option(URI, 'sockettimeoutms=500');
$manager = new MongoDB\Driver\Manager($uri);

$command = new MongoDB\Driver\Command([
    'sleep' => 1,
    'secs' => 1,
    'w' => false,
]);

echo throws(function() use ($manager, $command) {
    $manager->executeCommand('admin', $command);
}, 'MongoDB\Driver\Exception\\ConnectionTimeoutException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
Failed to send "sleep" command with database "admin": %Ssocket error or timeout
===DONE===

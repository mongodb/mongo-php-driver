--TEST--
MongoDB\Driver\Session::isDirty()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_failcommand_failpoint(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer();
$session = $manager->startSession();

printf("New session is dirty: %s\n", $session->isDirty() ? 'yes' : 'no');

$command = new MongoDB\Driver\Command(['ping' => 1]);
$server->executeCommand(DATABASE_NAME, $command, ['session' => $session]);

printf("Session after successful command is dirty: %s\n", $session->isDirty() ? 'yes' : 'no');

configureTargetedFailPoint($server, 'failCommand', [ 'times' => 1 ], [
    'failCommands' => ['ping'],
    'closeConnection' => true
]);

throws(function() use ($server, $command, $session) {
    $server->executeCommand(DATABASE_NAME, $command, ['session' => $session]);
}, MongoDB\Driver\Exception\ConnectionTimeoutException::class);

printf("Session after network error is dirty: %s\n", $session->isDirty() ? 'yes' : 'no');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
New session is dirty: no
Session after successful command is dirty: no
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
Session after network error is dirty: yes
===DONE===

--TEST--
MongoDB\Driver\Server::executeReadWriteCommand() with invalid options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY));

$command = new MongoDB\Driver\Command(['ping' => 1]);

echo throws(function() use ($server, $command) {
    $server->executeReadWriteCommand(DATABASE_NAME, $command, ['readConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($server, $command) {
    $server->executeReadWriteCommand(DATABASE_NAME, $command, ['readConcern' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($server, $command) {
    $server->executeReadWriteCommand(DATABASE_NAME, $command, ['session' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($server, $command) {
    $server->executeReadWriteCommand(DATABASE_NAME, $command, ['session' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($server, $command) {
    $server->executeReadWriteCommand(DATABASE_NAME, $command, ['writeConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($server, $command) {
    $server->executeReadWriteCommand(DATABASE_NAME, $command, ['writeConcern' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "session" option to be MongoDB\Driver\Session, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "session" option to be MongoDB\Driver\Session, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, stdClass given
===DONE===

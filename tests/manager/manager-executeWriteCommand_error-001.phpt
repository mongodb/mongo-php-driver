--TEST--
MongoDB\Driver\Manager::executeWriteCommand() with invalid options
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$command = new MongoDB\Driver\Command(['ping' => 1]);

echo throws(function() use ($manager, $command) {
    $manager->executeWriteCommand(DATABASE_NAME, $command, ['session' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $command) {
    $manager->executeWriteCommand(DATABASE_NAME, $command, ['session' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $command) {
    $manager->executeWriteCommand(DATABASE_NAME, $command, ['writeConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $command) {
    $manager->executeWriteCommand(DATABASE_NAME, $command, ['writeConcern' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "session" option to be MongoDB\Driver\Session, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "session" option to be MongoDB\Driver\Session, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "writeConcern" option to be MongoDB\Driver\WriteConcern, stdClass given
===DONE===

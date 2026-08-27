--TEST--
MongoDB\Driver\Manager::executeCommand() rejects a null byte or period in the database name
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

/* A null byte would truncate the database name at the C-string layer. */
echo throws(function() use ($manager) {
    $manager->executeCommand("data\0base", new MongoDB\Driver\Command(['ping' => 1]));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* A period would be read by the server as a namespace separator and retarget
 * the command to a different database. */
echo throws(function() use ($manager) {
    $manager->executeCommand("data.base", new MongoDB\Driver\Command(['ping' => 1]));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* The same validation applies to executeReadCommand() and executeWriteCommand(). */
echo throws(function() use ($manager) {
    $manager->executeReadCommand("data.base", new MongoDB\Driver\Command(['ping' => 1]));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager) {
    $manager->executeWriteCommand("data.base", new MongoDB\Driver\Command(['ping' => 1]));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid database name provided: database names may not contain a null byte
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid database name provided: database names may not contain a '.' character: data.base
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid database name provided: database names may not contain a '.' character: data.base
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid database name provided: database names may not contain a '.' character: data.base
===DONE===

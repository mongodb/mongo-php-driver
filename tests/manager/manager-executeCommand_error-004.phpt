--TEST--
MongoDB\Driver\Manager::executeCommand() with empty command document
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$command = new MongoDB\Driver\Command([]);

echo throws(function() use ($manager, $command) {
    $manager->executeCommand(DATABASE_NAME, $command);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Empty command document
===DONE===

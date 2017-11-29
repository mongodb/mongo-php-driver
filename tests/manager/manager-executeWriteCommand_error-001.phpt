--TEST--
MongoDB\Driver\Manager::executeWriteCommand()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$command = new MongoDB\Driver\Command([]);

echo throws(function() use ($manager, $command) {
	$manager->executeWriteCommand(
		DATABASE_NAME, $command,
		[
			'readConcern' => new \MongoDB\Driver\ReadConcern(\MongoDB\Driver\ReadConcern::MAJORITY),
		]
	);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $command) {
	$manager->executeWriteCommand(
		DATABASE_NAME, $command,
		[
			'readPreference' => new \MongoDB\Driver\ReadConcern(\MongoDB\Driver\ReadPreference::RP_NEAREST),
		]
	);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'readConcern'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'readPreference'
===DONE===

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

(new CommandObserver)->observe(
	function() use ($manager) {
		$command = new MongoDB\Driver\Command([
			'ping' => true,
		]);
		$manager->executeCommand(
			DATABASE_NAME,
			$command,
			[
				'readPreference' => new \MongoDB\Driver\ReadPreference(\MongoDB\Driver\ReadPreference::RP_SECONDARY),
				'readConcern' => new \MongoDB\Driver\ReadConcern(\MongoDB\Driver\ReadConcern::LOCAL),
				/* The ping command itself doesn't support writeConcern */
				// 'writeConcern' => new \MongoDB\Driver\WriteConcern(\MongoDB\Driver\WriteConcern::MAJORITY),
			]
		);
	},
	function(stdClass $command) {
		echo "Read Preference: ", $command->{'$readPreference'}->mode, "\n";
		echo "Read Concern: ", $command->readConcern->level, "\n";
		// echo "Write Concern: ", $command->writeConcern->w, "\n";
	}
);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Read Preference: secondary
Read Concern: local
===DONE===

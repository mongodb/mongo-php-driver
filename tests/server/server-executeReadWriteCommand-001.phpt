--TEST--
MongoDB\Driver\Manager::executeReadWriteCommand()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY));

(new CommandObserver)->observe(
    function() use ($server) {
        $command = new MongoDB\Driver\Command( [
            'findAndModify' => NS,
            'update' => [ '$set' => [ 'foo' => 'bar' ] ],
        ] );
        $server->executeReadWriteCommand(
            DATABASE_NAME,
            $command,
            [
                'readConcern' => new \MongoDB\Driver\ReadConcern(\MongoDB\Driver\ReadConcern::LOCAL),
                'writeConcern' => new \MongoDB\Driver\WriteConcern(\MongoDB\Driver\WriteConcern::MAJORITY),
            ]
        );
    },
    function(stdClass $command) {
        echo "Read Concern: ", $command->readConcern->level, "\n";
        echo "Write Concern: ", $command->writeConcern->w, "\n";
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Read Concern: local
Write Concern: majority
===DONE===

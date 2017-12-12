--TEST--
MongoDB\Driver\Manager::executeWriteCommand()
--SKIPIF--
<?php if (getenv("TRAVIS")) exit("skip This currently tails on Travis because it doesn't run 3.6 yet"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY));

$bw = new MongoDB\Driver\BulkWrite();
$bw->insert(['a' => 1]);
$manager->executeBulkWrite(NS, $bw);

(new CommandObserver)->observe(
    function() use ($server) {
        $command = new MongoDB\Driver\Command([
            'drop' => COLLECTION_NAME,
        ]);
        $server->executeWriteCommand(
            DATABASE_NAME,
            $command,
            [
                'writeConcern' => new \MongoDB\Driver\WriteConcern(\MongoDB\Driver\WriteConcern::MAJORITY),
            ]
        );
    },
    function(stdClass $command) {
        echo "Write Concern: ", $command->writeConcern->w, "\n";
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Write Concern: majority
===DONE===

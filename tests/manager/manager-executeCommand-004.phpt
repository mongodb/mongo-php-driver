--TEST--
MongoDB\Driver\Manager::executeCommand() options (MONGOC_CMD_RAW)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

$manager = create_test_manager();

(new CommandObserver)->observe(
    function() use ($manager) {
        $command = new MongoDB\Driver\Command([
            'ping' => true,
        ]);

        try {
            $manager->executeCommand(
                DATABASE_NAME,
                $command,
                [
                    'readPreference' => new \MongoDB\Driver\ReadPreference(\MongoDB\Driver\ReadPreference::SECONDARY),
                    'readConcern' => new \MongoDB\Driver\ReadConcern(\MongoDB\Driver\ReadConcern::LOCAL),
                    'writeConcern' => new \MongoDB\Driver\WriteConcern(\MongoDB\Driver\WriteConcern::MAJORITY),
                ]
            );
        } catch ( Exception $e ) {
            // Ignore exception that ping doesn't support writeConcern
        }
    },
    function(stdClass $command) {
        echo "Read Preference: ", $command->{'$readPreference'}->mode, "\n";
        echo "Read Concern: ", $command->readConcern->level, "\n";
        echo "Write Concern: ", $command->writeConcern->w, "\n";
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Read Preference: secondary
Read Concern: local
Write Concern: majority
===DONE===

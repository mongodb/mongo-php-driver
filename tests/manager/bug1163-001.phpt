--TEST--
PHPC-1163: Unacknowledged write concern should omit implicit session
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.4'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Test implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function run()
    {
        $manager = new MongoDB\Driver\Manager(URI, ['w' => 0]);

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $bulk = new MongoDB\Driver\BulkWrite;
        $bulk->insert(['x' => 1]);

        echo "Testing executeBulkWrite\n";
        $manager->executeBulkWrite(NS, $bulk);

        $command = new MongoDB\Driver\Command([
            'insert' => COLLECTION_NAME,
            'documents' => [['x' => 1]],
        ]);

        /* Note: executeCommand() and executeReadCommand() are not tested
         * because they do not inherit the client-level write concern. */

        echo "\nTesting executeWriteCommand\n";
        $manager->executeWriteCommand(DATABASE_NAME, $command);

        /* We can safely re-use the insert command with executeReadWriteCommand
         * because there is no readConcern to inherit. */
        echo "\nTesting executeReadWriteCommand\n";
        $manager->executeReadWriteCommand(DATABASE_NAME, $command);

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        if ($event->getCommandName() === 'insert') {
            $command = $event->getCommand();
            $hasSession = isset($command->lsid);
            $writeConcern = isset($command->writeConcern) ? $command->writeConcern: null;

            printf("insert command write concern: %s\n", json_encode($writeConcern));
            printf("insert command has session: %s\n", $hasSession ? 'yes' : 'no');
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

(new Test)->run();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing executeBulkWrite
insert command write concern: {"w":0}
insert command has session: no

Testing executeWriteCommand
insert command write concern: {"w":0}
insert command has session: no

Testing executeReadWriteCommand
insert command write concern: {"w":0}
insert command has session: no
===DONE===

--TEST--
Causal consistency: first read or write in session updates operationTime (even on error)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class Test implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    private $lastSeenOperationTime;

    public function executeBulkWrite()
    {
        $this->lastSeenOperationTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $bulk = new MongoDB\Driver\BulkWrite;
        $bulk->insert(['_id' => 1]);
        $bulk->insert(['_id' => 1]);

        throws(function() use ($manager, $bulk, $session) {
            $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);
        }, 'MongoDB\Driver\Exception\BulkWriteException');

        printf("Session reports last seen operationTime: %s\n", ($session->getOperationTime() == $this->lastSeenOperationTime) ? 'yes' : 'no');

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function executeCommand()
    {
        $this->lastSeenOperationTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $command = new MongoDB\Driver\Command([
            'aggregate' => COLLECTION_NAME,
            'pipeline' => [
                ['$unsupportedOperator' => 1],
            ],
            'cursor' => new stdClass,
        ]);

        throws(function() use ($manager, $command, $session) {
            $manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);
        }, 'MongoDB\Driver\Exception\RuntimeException');

        /* We cannot access the server reply if an exception is thrown for a
         * failed command (see: PHPC-1076). For the time being, just assert that
         * the operationTime is not null. */
        printf("Session has non-null operationTime: %s\n", ($session->getOperationTime() !== null ? 'yes' : 'no'));

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function executeQuery()
    {
        $this->lastSeenOperationTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $query = new MongoDB\Driver\Query(['$unsupportedOperator' => 1]);

        throws(function() use ($manager, $query, $session) {
            $manager->executeQuery(NS, $query, ['session' => $session]);
        }, 'MongoDB\Driver\Exception\RuntimeException');

        /* We cannot access the server reply if an exception is thrown for a
         * failed command (see: PHPC-1076). For the time being, just assert that
         * the operationTime is not null. */
        printf("Session has non-null operationTime: %s\n", ($session->getOperationTime() !== null ? 'yes' : 'no'));

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
        $reply = $event->getReply();
        $hasOperationTime = isset($reply->operationTime);

        printf("%s command reply includes operationTime: %s\n", $event->getCommandName(), $hasOperationTime ? 'yes' : 'no');

        if ($hasOperationTime) {
            $this->lastSeenOperationTime = $reply->operationTime;
        }
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

echo "Testing executeBulkWrite()\n";
(new Test)->executeBulkWrite();

echo "\nTesting executeCommand()\n";
(new Test)->executeCommand();

echo "\nTesting executeQuery()\n";
(new Test)->executeQuery();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing executeBulkWrite()
insert command reply includes operationTime: yes
OK: Got MongoDB\Driver\Exception\BulkWriteException
Session reports last seen operationTime: yes

Testing executeCommand()
OK: Got MongoDB\Driver\Exception\RuntimeException
Session has non-null operationTime: yes

Testing executeQuery()
OK: Got MongoDB\Driver\Exception\RuntimeException
Session has non-null operationTime: yes
===DONE===

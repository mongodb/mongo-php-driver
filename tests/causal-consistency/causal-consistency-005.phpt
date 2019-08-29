--TEST--
Causal consistency: second read's afterClusterTime uses last reply's operationTime
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

    public function executeReadAfterRead()
    {
        $this->lastSeenOperationTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $query = new MongoDB\Driver\Query([]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
        $manager->executeQuery(NS, $query, ['session' => $session]);

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function executeReadAfterWrite()
    {
        $this->lastSeenOperationTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $bulk = new MongoDB\Driver\BulkWrite;
        $bulk->insert(['x' => 1]);
        $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);

        $query = new MongoDB\Driver\Query([]);
        $manager->executeQuery(NS, $query, ['session' => $session]);

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $command = $event->getCommand();
        $hasAfterClusterTime = isset($command->readConcern->afterClusterTime);
        printf("%s command includes afterClusterTime: %s\n", $event->getCommandName(), ($hasAfterClusterTime ? 'yes' : 'no'));

        if ($hasAfterClusterTime && $this->lastSeenOperationTime !== null) {
            printf("%s command uses last seen operationTime: %s\n", $event->getCommandName(), ($command->readConcern->afterClusterTime == $this->lastSeenOperationTime) ? 'yes' : 'no');
        }
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

echo "Testing read after read\n";
(new Test)->executeReadAfterRead();

echo "\nTesting read after write\n";
(new Test)->executeReadAfterWrite();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing read after read
find command includes afterClusterTime: no
find command reply includes operationTime: yes
find command includes afterClusterTime: yes
find command uses last seen operationTime: yes
find command reply includes operationTime: yes

Testing read after write
insert command includes afterClusterTime: no
insert command reply includes operationTime: yes
find command includes afterClusterTime: yes
find command uses last seen operationTime: yes
find command reply includes operationTime: yes
===DONE===

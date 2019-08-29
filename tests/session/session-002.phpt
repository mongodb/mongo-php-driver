--TEST--
MongoDB\Driver\Session spec test: $clusterTime in commands
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
    private $lastSeenClusterTime;

    public function aggregate()
    {
        $this->lastSeenClusterTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $command = new MongoDB\Driver\Command([
            'aggregate' => COLLECTION_NAME,
            'pipeline' => [],
            'cursor' => new stdClass(),
        ]);
        $manager->executeReadWriteCommand(DATABASE_NAME, $command, ['session' => $session]);
        $manager->executeReadWriteCommand(DATABASE_NAME, $command, ['session' => $session]);

        printf("Session reports last seen \$clusterTime: %s\n", ($session->getClusterTime() == $this->lastSeenClusterTime) ? 'yes' : 'no');

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function find()
    {
        $this->lastSeenClusterTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $query = new MongoDB\Driver\Query([]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
        $manager->executeQuery(NS, $query, ['session' => $session]);

        printf("Session reports last seen \$clusterTime: %s\n", ($session->getClusterTime() == $this->lastSeenClusterTime) ? 'yes' : 'no');

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function insert()
    {
        $this->lastSeenClusterTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $bulk = new MongoDB\Driver\BulkWrite();
        $bulk->insert(['x' => 1]);
        $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);

        $bulk = new MongoDB\Driver\BulkWrite();
        $bulk->insert(['x' => 2]);
        $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);

        printf("Session reports last seen \$clusterTime: %s\n", ($session->getClusterTime() == $this->lastSeenClusterTime) ? 'yes' : 'no');

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function ping()
    {
        $this->lastSeenClusterTime = null;

        MongoDB\Driver\Monitoring\addSubscriber($this);

        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $command = new MongoDB\Driver\Command(['ping' => 1]);
        $manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);
        $manager->executeCommand(DATABASE_NAME, $command, ['session' => $session]);

        printf("Session reports last seen \$clusterTime: %s\n", ($session->getClusterTime() == $this->lastSeenClusterTime) ? 'yes' : 'no');

        MongoDB\Driver\Monitoring\removeSubscriber($this);
    }

    public function commandStarted(MongoDB\Driver\Monitoring\CommandStartedEvent $event)
    {
        $command = $event->getCommand();
        $hasClusterTime = isset($command->{'$clusterTime'});

        printf("%s command includes \$clusterTime: %s\n", $event->getCommandName(), $hasClusterTime ? 'yes' : 'no');

        if ($hasClusterTime && $this->lastSeenClusterTime !== null) {
            printf("%s command uses last seen \$clusterTime: %s\n", $event->getCommandName(), ($command->{'$clusterTime'} == $this->lastSeenClusterTime) ? 'yes' : 'no');
        }
    }

    public function commandSucceeded(MongoDB\Driver\Monitoring\CommandSucceededEvent $event)
    {
        $reply = $event->getReply();
        $hasClusterTime = isset($reply->{'$clusterTime'});

        printf("%s command reply includes \$clusterTime: %s\n", $event->getCommandName(), $hasClusterTime ? 'yes' : 'no');

        if ($hasClusterTime) {
            $this->lastSeenClusterTime = $reply->{'$clusterTime'};
        }
    }

    public function commandFailed(MongoDB\Driver\Monitoring\CommandFailedEvent $event)
    {
    }
}

echo "\nTesting aggregate command\n";
(new Test)->aggregate();

echo "\nTesting find command\n";
(new Test)->find();

echo "\nTesting insert command\n";
(new Test)->insert();

echo "\nTesting ping command\n";
(new Test)->ping();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing aggregate command
aggregate command includes $clusterTime: yes
aggregate command reply includes $clusterTime: yes
aggregate command includes $clusterTime: yes
aggregate command uses last seen $clusterTime: yes
aggregate command reply includes $clusterTime: yes
Session reports last seen $clusterTime: yes

Testing find command
find command includes $clusterTime: yes
find command reply includes $clusterTime: yes
find command includes $clusterTime: yes
find command uses last seen $clusterTime: yes
find command reply includes $clusterTime: yes
Session reports last seen $clusterTime: yes

Testing insert command
insert command includes $clusterTime: yes
insert command reply includes $clusterTime: yes
insert command includes $clusterTime: yes
insert command uses last seen $clusterTime: yes
insert command reply includes $clusterTime: yes
Session reports last seen $clusterTime: yes

Testing ping command
ping command includes $clusterTime: yes
ping command reply includes $clusterTime: yes
ping command includes $clusterTime: yes
ping command uses last seen $clusterTime: yes
ping command reply includes $clusterTime: yes
Session reports last seen $clusterTime: yes
===DONE===

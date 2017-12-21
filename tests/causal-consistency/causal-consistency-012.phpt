--TEST--
Causal consistency: $clusterTime is sent in commands to supported deployments
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

(new CommandObserver)->observe(
    function() {
        $manager = new MongoDB\Driver\Manager(REPLICASET);
        $session = $manager->startSession();

        $query = new MongoDB\Driver\Query([]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
    },
    function(stdClass $command)
    {
        $hasClusterTime = isset($command->{'$clusterTime'});
        printf("Command includes \$clusterTime: %s\n", ($hasClusterTime ? 'yes' : 'no'));
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Command includes $clusterTime: yes
Command includes $clusterTime: yes
===DONE===

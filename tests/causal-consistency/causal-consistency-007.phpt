--TEST--
Causal consistency: reads in non-causally consistent session never include afterClusterTime
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
        $session = $manager->startSession(['causalConsistency' => false]);

        $query = new MongoDB\Driver\Query([]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
    },
    function(stdClass $command)
    {
        $hasAfterClusterTime = isset($command->readConcern->afterClusterTime);
        printf("Read includes afterClusterTime: %s\n", ($hasAfterClusterTime ? 'yes' : 'no'));
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Read includes afterClusterTime: no
Read includes afterClusterTime: no
===DONE===

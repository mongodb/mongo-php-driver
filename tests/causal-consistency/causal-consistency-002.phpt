--TEST--
Causal consistency: first read in session does not include afterClusterTime
--DESCRIPTION--
Causal consistency spec prose test #2
https://github.com/mongodb/specifications/blob/master/source/causal-consistency/causal-consistency.rst#test-plan
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_sharded_cluster_with_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

(new CommandObserver)->observe(
    function() {
        $manager = create_test_manager();
        $session = $manager->startSession();

        $query = new MongoDB\Driver\Query([]);
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
===DONE===

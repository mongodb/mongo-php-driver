--TEST--
Causal consistency: $clusterTime is sent in commands to supported deployments
--DESCRIPTION--
Causal consistency spec prose test #12
https://github.com/mongodb/specifications/blob/master/source/causal-consistency/causal-consistency.rst#test-plan
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_load_balanced(); /* TODO: CDRIVER-4174 */ ?>
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

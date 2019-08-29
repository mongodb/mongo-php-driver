--TEST--
Causal consistency: custom read concern merges afterClusterTime and level
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_replica_set_or_mongos_with_replica_set(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/observer.php";

(new CommandObserver)->observe(
    function() {
        $manager = new MongoDB\Driver\Manager(URI);
        $session = $manager->startSession();

        $readConcern = new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY);
        $query = new MongoDB\Driver\Query([], ['readConcern' => $readConcern]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
        $manager->executeQuery(NS, $query, ['session' => $session]);
    },
    function(stdClass $command)
    {
        $hasAfterClusterTime = isset($command->readConcern->afterClusterTime);
        printf("Read concern includes afterClusterTime: %s\n", ($hasAfterClusterTime ? 'yes' : 'no'));

        $hasLevel = isset($command->readConcern->level);
        printf("Read concern includes level: %s\n", ($hasLevel ? 'yes' : 'no'));
    }
);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Read concern includes afterClusterTime: no
Read concern includes level: yes
Read concern includes afterClusterTime: yes
Read concern includes level: yes
===DONE===

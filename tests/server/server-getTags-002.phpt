--TEST--
MongoDB\Driver\Server::getTags() with replica set
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command);

function assertSomeServerHasTags(array $servers, array $expectedTags) {
    foreach ($servers as $server) {
        /* Using a non-strict comparison guards against tags being returned in
         * a different order than expected. */
        if ($expectedTags == $server->getTags()) {
            printf("Found server with tags: %s\n", json_encode($expectedTags));
            return;
        }
    }

    printf("No server has tags: %s\n", json_encode($expectedTags));
}

$servers = $manager->getServers();
assertSomeServerHasTags($servers, ['dc' => 'ny', 'ordinal' => 'one']);
assertSomeServerHasTags($servers, ['dc' => 'pa', 'ordinal' => 'two']);
assertSomeServerHasTags($servers, []);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Found server with tags: {"dc":"ny","ordinal":"one"}
Found server with tags: {"dc":"pa","ordinal":"two"}
Found server with tags: []
===DONE===

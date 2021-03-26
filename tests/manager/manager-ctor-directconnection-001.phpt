--TEST--
MongoDB\Driver\Manager::__construct(): directConnection option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_enough_data_nodes(2); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager(URI, ['directConnection' => false]);
$server = $manager->selectServer(new \MongoDB\Driver\ReadPreference('primaryPreferred'));

printf("Topology has multiple nodes when directConnection=false: %s\n", count($manager->getServers()) > 1 ? 'true' : 'false');

$uri = sprintf('mongodb://%s:%d', $server->getHost(), $server->getPort());
$manager2 = create_test_manager($uri, ['directConnection' => true]);
$server2 = $manager2->selectServer(new \MongoDB\Driver\ReadPreference('primaryPreferred'));

printf("Topology has single node when directConnection=true: %s\n", count($manager2->getServers()) == 1 ? 'true' : 'false');
printf("Single node in topology matches seed in URI: %s\n", ($server2 == $server) ? 'true' : 'false');

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Topology has multiple nodes when directConnection=false: true
Topology has single node when directConnection=true: true
Single node in topology matches seed in URI: true
===DONE===

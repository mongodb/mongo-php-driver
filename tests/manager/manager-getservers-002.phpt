--TEST--
MongoDB\Driver\Manager::getServers() (replica set)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_enough_nodes(3); ?>
<?php skip_if_no_arbiter(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function assertServerType($type) {
    $rsTypes = array(
        MongoDB\Driver\Server::TYPE_RS_PRIMARY,
        MongoDB\Driver\Server::TYPE_RS_SECONDARY,
        MongoDB\Driver\Server::TYPE_RS_ARBITER,
    );

    if (in_array($type, $rsTypes, true)) {
        printf("Found replica set server type: %d\n", $type);
    } else {
        printf("Unexpected server type: %d\n", $type);
    }
}

$manager = new MongoDB\Driver\Manager(URI);

$servers = $manager->getServers();
printf("Known servers: %d\n", count($servers));

echo "Pinging\n";
$command = new MongoDB\Driver\Command(array('ping' => 1));
$manager->executeCommand(DATABASE_NAME, $command);

$servers = $manager->getServers();
printf("Known servers: %d\n", count($servers));

foreach ($servers as $server) {
    printf("Found server: %s:%d\n", $server->getHost(), $server->getPort());
    assertServerType($server->getType());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Known servers: 0
Pinging
Known servers: 3
Found server: %s:%d
Found replica set server type: %r(4|5|6)%r
Found server: %s:%d
Found replica set server type: %r(4|5|6)%r
Found server: %s:%d
Found replica set server type: %r(4|5|6)%r
===DONE===

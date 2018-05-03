--TEST--
MongoDB\Driver\Manager::getServers() (standalone)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_standalone(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

function assertServerType($type) {
    if ($type === MongoDB\Driver\Server::TYPE_STANDALONE) {
        printf("Found standalone server type: %d\n", $type);
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
Known servers: 1
Found server: %s:%d
Found standalone server type: 1
===DONE===

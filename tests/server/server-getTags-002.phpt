--TEST--
MongoDB\Driver\Server::getTags() with replica set
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand(DATABASE_NAME, $command);

foreach ($manager->getServers() as $server) {
    $tags = $server->getTags();
    echo "dc: ", array_key_exists('dc', $tags) ? $tags['dc'] : 'not set', "\n";
    echo "ordinal: ", array_key_exists('ordinal', $tags) ? $tags['ordinal'] : 'not set', "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
dc: ny
ordinal: one
dc: pa
ordinal: two
dc: not set
ordinal: not set
===DONE===

--TEST--
MongoDB\Driver\Server::getLatency() returns null when unset (e.g. load balancer)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_load_balanced(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));
var_dump($server->getLatency());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
===DONE===

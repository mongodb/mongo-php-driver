--TEST--
MongoDB\Driver\Server::__construct()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

/* For replica sets we need to do some extra work to get the primary */
$uri = is_replica_set(URI) ? get_primary_server(URI)->getInfo()['me'] : URI;
$parsed = parse_url($uri);

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert(array('foo' => 'bar'));
$server = $manager->executeBulkWrite(NS, $bulk)->getServer();

var_dump($server->getHost() == $parsed["host"]);
var_dump($server->getPort() == $parsed["port"]);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
===DONE===

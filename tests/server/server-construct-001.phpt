--TEST--
MongoDB\Driver\Server::__construct()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_multiple_mongos(); ?>
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

$expectedHost = $parsed['host'];
$expectedPort = (integer) (isset($parsed['port']) ? $parsed['port'] : 27017);

var_dump($server->getHost() == $expectedHost);
var_dump($server->getPort() == $expectedPort);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
===DONE===

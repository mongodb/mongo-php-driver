--TEST--
MongoDB\Driver\Server::executeQuery() with sort and empty filter
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()))->getServer();

var_dump($server->getHost(true));
var_dump($server->getTags(true));
var_dump($server->getInfo(true));
var_dump($server->getLatency(true));
var_dump($server->getPort(true));
var_dump($server->getType(true));
var_dump($server->isPrimary(true));
var_dump($server->isSecondary(true));
var_dump($server->isArbiter(true));
var_dump($server->isHidden(true));
var_dump($server->isPassive(true));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Warning: MongoDB\Driver\Server::getHost() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::getTags() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::getInfo() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::getLatency() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::getPort() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::getType() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::isPrimary() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::isSecondary() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::isArbiter() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::isHidden() expects exactly 0 parameters, 1 given in %s on line %d
NULL

Warning: MongoDB\Driver\Server::isPassive() expects exactly 0 parameters, 1 given in %s on line %d
NULL
===DONE===

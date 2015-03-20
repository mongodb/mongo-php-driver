--TEST--
MongoDB\Driver\Server::executeCommand()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
$server = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()))->getServer();

$command = new MongoDB\Driver\Command(array('isMaster' => 1));
$result = $server->executeCommand(DATABASE_NAME, $command);

var_dump($result instanceof MongoDB\Driver\Result);

$responseDocument = $result->toArray();

var_dump( ! empty($responseDocument['ok']));
var_dump($server == $result->getServer());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
===DONE===

--TEST--
MongoDB\Server::executeCommand()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$server = new MongoDB\Server('localhost', 27017);

$command = new MongoDB\Command(array('isMaster' => 1));
$result = $server->executeCommand(DATABASE_NAME, $command);

var_dump($result instanceof MongoDB\CommandResult);

$responseDocument = $result->getResponseDocument();

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

--TEST--
MongoDB\Manager::executeCommand()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

$command = new MongoDB\Command(array('isMaster' => 1));
$result = $manager->executeCommand(DATABASE_NAME, $command);

var_dump($result instanceof MongoDB\CommandResult);

$responseDocument = $result->getResponseDocument();

var_dump( ! empty($responseDocument['ok']));

$server = $result->getServer();

var_dump($server instanceof MongoDB\Server);
var_dump($server->getHost());
var_dump($server->getPort());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
bool(true)
string(9) "localhost"
int(27017)
===DONE===

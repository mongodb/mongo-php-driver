--TEST--
MongoDB\Driver\Server::executeCommand()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$parsed = parse_url(MONGODB_URI);
$server = new MongoDB\Driver\Server($parsed["host"], $parsed["port"]);

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

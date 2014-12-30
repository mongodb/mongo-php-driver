--TEST--
MongoDB\Driver\Server::__construct()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$parsed = parse_url(MONGODB_URI);
$server = new MongoDB\Driver\Server($parsed["host"], $parsed["port"]);

var_dump($server->getHost() == $parsed["host"]);
var_dump($server->getPort() == $parsed["port"]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
===DONE===

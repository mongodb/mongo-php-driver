--TEST--
MongoDB\Server::__construct()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$server = new MongoDB\Server('localhost', 27017);

var_dump($server->getHost());
var_dump($server->getPort());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(9) "localhost"
int(27017)
===DONE===

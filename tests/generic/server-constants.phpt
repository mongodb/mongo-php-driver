--TEST--
MongoDB\Driver\Server constants
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

var_dump(MongoDB\Driver\Server::TYPE_MONGOS);
var_dump(MongoDB\Driver\Server::TYPE_STANDALONE);
var_dump(MongoDB\Driver\Server::TYPE_ARBITER);
var_dump(MongoDB\Driver\Server::TYPE_SECONDARY);
var_dump(MongoDB\Driver\Server::TYPE_PRIMARY);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(1)
int(2)
int(3)
int(4)
int(5)
===DONE===

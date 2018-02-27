--TEST--
MongoDB\Driver\Server constants
--FILE--
<?php

var_dump(MongoDB\Driver\Server::TYPE_UNKNOWN);
var_dump(MongoDB\Driver\Server::TYPE_STANDALONE);
var_dump(MongoDB\Driver\Server::TYPE_MONGOS);
var_dump(MongoDB\Driver\Server::TYPE_POSSIBLE_PRIMARY);
var_dump(MongoDB\Driver\Server::TYPE_RS_PRIMARY);
var_dump(MongoDB\Driver\Server::TYPE_RS_SECONDARY);
var_dump(MongoDB\Driver\Server::TYPE_RS_ARBITER);
var_dump(MongoDB\Driver\Server::TYPE_RS_OTHER);
var_dump(MongoDB\Driver\Server::TYPE_RS_GHOST);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
int(1)
int(2)
int(3)
int(4)
int(5)
int(6)
int(7)
int(8)
===DONE===

--TEST--
MongoDB\Query\Query constants
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

var_dump(MongoDB\Query\Query::FLAG_TAILABLE_CURSOR);
var_dump(MongoDB\Query\Query::FLAG_SLAVE_OK);
var_dump(MongoDB\Query\Query::FLAG_OPLOG_REPLAY);
var_dump(MongoDB\Query\Query::FLAG_NO_CURSOR_TIMEOUT);
var_dump(MongoDB\Query\Query::FLAG_AWAIT_DATA);
var_dump(MongoDB\Query\Query::FLAG_EXHAUST);
var_dump(MongoDB\Query\Query::FLAG_PARTIAL);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(2)
int(4)
int(8)
int(16)
int(32)
int(64)
int(128)
===DONE===

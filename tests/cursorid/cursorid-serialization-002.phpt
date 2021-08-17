--TEST--
MongoDB\Driver\CursorId serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$serialized = 'O:23:"MongoDB\Driver\CursorId":1:{s:2:"id";s:19:"7250031947823432848";}';

$cursorId = unserialize($serialized);

var_dump($cursorId);
var_dump($cursorId instanceof Serializable);
echo serialize($cursorId), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  %rint\(\d+\)|string\(\d+\) "\d+"%r
}
bool(true)
O:23:"MongoDB\Driver\CursorId":1:{s:2:"id";s:19:"7250031947823432848";}
===DONE===

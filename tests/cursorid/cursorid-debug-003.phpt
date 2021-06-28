--TEST--
MongoDB\Driver\CursorId debug output on 64-bit platform
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$cursorId = unserialize('C:23:"MongoDB\Driver\CursorId":42:{a:1:{s:2:"id";s:19:"7250031947823432848";}}');

var_dump($cursorId);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  int(7250031947823432848)
}
===DONE===

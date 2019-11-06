--TEST--
MongoDB\Driver\CursorID serialization
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$serialized = 'C:23:"MongoDB\Driver\CursorId":42:{a:1:{s:2:"id";s:19:"7250031947823432848";}}';

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
C:23:"MongoDB\Driver\CursorId":42:{a:1:{s:2:"id";s:19:"7250031947823432848";}}
===DONE===

--TEST--
MongoDB\Driver\CursorId::__toString()
--FILE--
<?php

$cursorId = unserialize('C:23:"MongoDB\Driver\CursorId":42:{a:1:{s:2:"id";s:19:"7250031947823432848";}}');

var_dump((string) $cursorId);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
string(19) "7250031947823432848"
===DONE===

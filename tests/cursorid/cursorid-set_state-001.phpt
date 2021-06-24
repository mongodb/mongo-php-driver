--TEST--
MongoDB\Driver\CursorId::__set_state()
--FILE--
<?php

$cursorId = unserialize('C:23:"MongoDB\Driver\CursorId":42:{a:1:{s:2:"id";s:19:"7250031947823432848";}}');

var_export(MongoDB\Driver\CursorId::__set_state([
    'id' => "$cursorId",
]));
echo "\n\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\CursorId::__set_state(array(
%w'id' => '7250031947823432848',
))

===DONE===

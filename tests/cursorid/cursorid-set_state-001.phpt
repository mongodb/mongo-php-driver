--TEST--
MongoDB\Driver\CursorId::__set_state()
--FILE--
<?php

var_export(MongoDB\Driver\CursorId::__set_state([
    'id' => '7250031947823432848',
]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%r\\?%rMongoDB\Driver\CursorId::__set_state(array(
   'id' => %r(7250031947823432848|'7250031947823432848')%r,
))
===DONE===

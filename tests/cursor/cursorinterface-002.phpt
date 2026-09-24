--TEST--
MongoDB\Driver\CursorInterface extends Iterator
--FILE--
<?php
var_dump(in_array('Iterator', class_implements('MongoDB\Driver\CursorInterface')));
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

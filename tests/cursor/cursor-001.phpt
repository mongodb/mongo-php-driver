--TEST--
MongoDB\Driver\Cursor implements MongoDB\Driver\CursorInterface and Traversable
--FILE--
<?php
var_dump(in_array('Traversable', class_implements('MongoDB\Driver\Cursor')));
var_dump(in_array('MongoDB\Driver\CursorInterface', class_implements('MongoDB\Driver\Cursor')));
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
===DONE===

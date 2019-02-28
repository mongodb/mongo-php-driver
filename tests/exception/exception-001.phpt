--TEST--
MongoDB\Driver\Exception\Exception extends Throwable
--SKIPIF--
<?php if (!interface_exists('Throwable')) { die('skip Throwable is not available'); } ?>
--FILE--
<?php
var_dump(in_array('Throwable', class_implements('MongoDB\Driver\Exception\Exception')));
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

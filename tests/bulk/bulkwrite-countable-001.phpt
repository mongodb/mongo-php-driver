--TEST--
MongoDB\Driver\BulkWrite implements Countable
--FILE--
<?php

$bulk = new MongoDB\Driver\BulkWrite;
var_dump($bulk instanceof Countable);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

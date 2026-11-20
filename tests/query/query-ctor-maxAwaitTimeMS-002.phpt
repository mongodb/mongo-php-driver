--TEST--
MongoDB\Driver\Query construction with maxAwaitTimeMS option (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$q = new MongoDB\Driver\Query([], ['maxAwaitTimeMS' => 4294967295]);
echo "maxAwaitTimeMS=4294967295: OK\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
maxAwaitTimeMS=4294967295: OK
===DONE===

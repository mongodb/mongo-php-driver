--TEST--
MongoDB\Driver\Query construction with maxAwaitTimeMS option
--FILE--
<?php

$q = new MongoDB\Driver\Query([], ['maxAwaitTimeMS' => 0]);
echo "maxAwaitTimeMS=0: OK\n";

$q = new MongoDB\Driver\Query([], ['maxAwaitTimeMS' => 1000]);
echo "maxAwaitTimeMS=1000: OK\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
maxAwaitTimeMS=0: OK
maxAwaitTimeMS=1000: OK
===DONE===

--TEST--
BSON BSON\Timestamp constructor requires 64-bit integers to be positive unsigned 32-bit integers
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

use MongoDB\BSON as BSON;

$tests = [
    new BSON\Timestamp(4294967295, 0),
    new BSON\Timestamp(0, 4294967295),
];

foreach ($tests as $test) {
    printf("Test %s\n", $test);
    var_dump($test);
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Test [4294967295:0]
object(%SBSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(4294967295)
  ["timestamp"]=>
  int(0)
}

Test [0:4294967295]
object(%SBSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(0)
  ["timestamp"]=>
  int(4294967295)
}

===DONE===

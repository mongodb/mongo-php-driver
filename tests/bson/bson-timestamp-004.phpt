--TEST--
MongoDB\BSON\Timestamp constructor requires 64-bit integers to be positive unsigned 32-bit integers
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Timestamp(4294967295, 0),
    new MongoDB\BSON\Timestamp(0, 4294967295),
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
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(10) "4294967295"
  ["timestamp"]=>
  string(1) "0"
}

Test [0:4294967295]
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "0"
  ["timestamp"]=>
  string(10) "4294967295"
}

===DONE===

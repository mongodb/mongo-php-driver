--TEST--
BSON BSON\Timestamp constructor requires positive unsigned 32-bit integers
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Timestamp(2147483647, 0),
    new MongoDB\BSON\Timestamp(0, 2147483647),
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
Test [2147483647:0]
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(2147483647)
  ["timestamp"]=>
  int(0)
}

Test [0:2147483647]
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(0)
  ["timestamp"]=>
  int(2147483647)
}

===DONE===
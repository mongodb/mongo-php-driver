--TEST--
MongoDB\BSON\Timestamp constructor requires positive unsigned 32-bit integers (as string)
--FILE--
<?php

$tests = [
    new MongoDB\BSON\Timestamp("2147483647", "0"),
    new MongoDB\BSON\Timestamp(0, "2147483647"),
    new MongoDB\BSON\Timestamp("4294967295", "0"),
    new MongoDB\BSON\Timestamp("0", "4294967295"),
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
  string(10) "2147483647"
  ["timestamp"]=>
  string(1) "0"
}

Test [0:2147483647]
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "0"
  ["timestamp"]=>
  string(10) "2147483647"
}

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

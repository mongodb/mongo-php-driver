--TEST--
MongoDB\BSON\Timestamp serialization
--FILE--
<?php

$tests = [
    [1234, 5678],
    [2147483647, 0],
    [0, 2147483647],
];

foreach ($tests as $test) {
    list($increment, $timestamp) = $test;

    var_dump($ts = new MongoDB\BSON\Timestamp($increment, $timestamp));
    var_dump($s = serialize($ts));
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(1234)
  ["timestamp"]=>
  int(5678)
}
string(80) "O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:1234;s:9:"timestamp";i:5678;}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(1234)
  ["timestamp"]=>
  int(5678)
}

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(2147483647)
  ["timestamp"]=>
  int(0)
}
string(83) "O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:2147483647;s:9:"timestamp";i:0;}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(2147483647)
  ["timestamp"]=>
  int(0)
}

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(0)
  ["timestamp"]=>
  int(2147483647)
}
string(83) "O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:0;s:9:"timestamp";i:2147483647;}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(0)
  ["timestamp"]=>
  int(2147483647)
}

===DONE===

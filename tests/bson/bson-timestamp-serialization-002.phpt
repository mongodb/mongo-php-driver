--TEST--
MongoDB\BSON\Timestamp serialization (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$tests = [
    [4294967295, 0],
    [0, 4294967295],
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
  string(10) "4294967295"
  ["timestamp"]=>
  string(1) "0"
}
string(99) "C:22:"MongoDB\BSON\Timestamp":64:{a:2:{s:9:"increment";s:10:"4294967295";s:9:"timestamp";s:1:"0";}}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(10) "4294967295"
  ["timestamp"]=>
  string(1) "0"
}

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "0"
  ["timestamp"]=>
  string(10) "4294967295"
}
string(99) "C:22:"MongoDB\BSON\Timestamp":64:{a:2:{s:9:"increment";s:1:"0";s:9:"timestamp";s:10:"4294967295";}}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "0"
  ["timestamp"]=>
  string(10) "4294967295"
}

===DONE===

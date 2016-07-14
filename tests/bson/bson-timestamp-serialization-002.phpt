--TEST--
MongoDB\BSON\Timestamp serialization (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

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
  int(4294967295)
  ["timestamp"]=>
  int(0)
}
string(83) "O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:4294967295;s:9:"timestamp";i:0;}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(4294967295)
  ["timestamp"]=>
  int(0)
}

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(0)
  ["timestamp"]=>
  int(4294967295)
}
string(83) "O:22:"MongoDB\BSON\Timestamp":2:{s:9:"increment";i:0;s:9:"timestamp";i:4294967295;}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(0)
  ["timestamp"]=>
  int(4294967295)
}

===DONE===

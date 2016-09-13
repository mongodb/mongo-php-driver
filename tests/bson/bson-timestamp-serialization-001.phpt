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
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}
string(95) "C:22:"MongoDB\BSON\Timestamp":60:{a:2:{s:9:"increment";s:4:"1234";s:9:"timestamp";s:4:"5678";}}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(4) "1234"
  ["timestamp"]=>
  string(4) "5678"
}

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(10) "2147483647"
  ["timestamp"]=>
  string(1) "0"
}
string(99) "C:22:"MongoDB\BSON\Timestamp":64:{a:2:{s:9:"increment";s:10:"2147483647";s:9:"timestamp";s:1:"0";}}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(10) "2147483647"
  ["timestamp"]=>
  string(1) "0"
}

object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "0"
  ["timestamp"]=>
  string(10) "2147483647"
}
string(99) "C:22:"MongoDB\BSON\Timestamp":64:{a:2:{s:9:"increment";s:1:"0";s:9:"timestamp";s:10:"2147483647";}}"
object(MongoDB\BSON\Timestamp)#%d (%d) {
  ["increment"]=>
  string(1) "0"
  ["timestamp"]=>
  string(10) "2147483647"
}

===DONE===

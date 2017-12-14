--TEST--
MongoDB\BSON\Symbol serialization
--FILE--
<?php

$test = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "symbol": {"$symbol": "symbolValue"} }'))->symbol;

var_dump($symbol = $test);
var_dump($s = serialize($symbol));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Symbol)#1 (1) {
  ["symbol"]=>
  string(11) "symbolValue"
}
string(70) "C:19:"MongoDB\BSON\Symbol":38:{a:1:{s:6:"symbol";s:11:"symbolValue";}}"
object(MongoDB\BSON\Symbol)#2 (1) {
  ["symbol"]=>
  string(11) "symbolValue"
}
===DONE===

--TEST--
MongoDB\BSON\Regex serialization
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump($regex = new MongoDB\BSON\Regex('regexp', 'i'));
var_dump($s = serialize($regex));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
string(77) "O:18:"MongoDB\BSON\Regex":2:{s:7:"pattern";s:6:"regexp";s:5:"flags";s:1:"i";}"
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
===DONE===

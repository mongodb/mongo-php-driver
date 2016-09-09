--TEST--
MongoDB\BSON\Regex serialization with flags omitted
--FILE--
<?php

var_dump($regex = new MongoDB\BSON\Regex('regexp'));
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
  string(0) ""
}
string(83) "C:18:"MongoDB\BSON\Regex":52:{a:2:{s:7:"pattern";s:6:"regexp";s:5:"flags";s:0:"";}}"
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(0) ""
}
===DONE===

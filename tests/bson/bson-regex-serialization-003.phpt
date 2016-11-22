--TEST--
MongoDB\BSON\Regex unserialization will alphabetize flags
--FILE--
<?php

var_dump(unserialize('C:18:"MongoDB\BSON\Regex":58:{a:2:{s:7:"pattern";s:6:"regexp";s:5:"flags";s:6:"xusmli";}}'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(6) "ilmsux"
}
===DONE===

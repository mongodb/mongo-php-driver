--TEST--
MongoDB\BSON\Regex debug handler
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('regexp', 'i');

var_dump($regex);

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
===DONE===

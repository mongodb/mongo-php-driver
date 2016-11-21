--TEST--
MongoDB\BSON\Regex initialization will alphabetize flags
--FILE--
<?php

$regex = new MongoDB\BSON\Regex('regexp', 'xusmli');

var_dump($regex);

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

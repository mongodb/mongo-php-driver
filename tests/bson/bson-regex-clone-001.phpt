--TEST--
MongoDB\BSON\Regex can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$regexp = new MongoDB\BSON\Regex("regexp", "i");

$clone = clone $regexp;

var_dump($clone == $regexp);
var_dump($clone === $regexp);

unset($regexp);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Regex)#%d (2) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
===DONE===

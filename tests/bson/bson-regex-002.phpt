--TEST--
BSON BSON\Regex debug handler
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . '\Regex';
$regex = new $classname('regexp', 'i');

var_dump($regex);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(%SBSON\Regex)#%d (%d) {
  ["pattern"]=>
  string(6) "regexp"
  ["flags"]=>
  string(1) "i"
}
===DONE===

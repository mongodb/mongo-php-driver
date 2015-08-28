--TEST--
BSON BSON\Timestamp debug handler
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . '\Timestamp';
$timestamp = new $classname(1234, 5678);

var_dump($timestamp);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(%SBSON\Timestamp)#%d (%d) {
  ["increment"]=>
  int(1234)
  ["timestamp"]=>
  int(5678)
}
===DONE===

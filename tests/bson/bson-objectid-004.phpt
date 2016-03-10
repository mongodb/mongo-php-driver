--TEST--
BSON BSON\ObjectID #004 Constructor supports uppercase hexadecimal strings
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

var_dump(new BSON\ObjectID('56925B7330616224D0000001'));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "56925b7330616224d0000001"
}
===DONE===

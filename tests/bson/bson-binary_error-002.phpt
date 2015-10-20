--TEST--
BSON BSON\Binary cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyBinary extends BSON\Binary {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyBinary may not inherit from final class (%SBSON\Binary) in %s on line %d

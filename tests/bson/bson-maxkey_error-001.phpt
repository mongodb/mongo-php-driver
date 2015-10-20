--TEST--
BSON BSON\MaxKey cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyMaxKey extends BSON\MaxKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMaxKey may not inherit from final class (%SBSON\MaxKey) in %s on line %d

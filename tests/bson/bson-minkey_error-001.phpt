--TEST--
BSON BSON\MinKey cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyMinKey extends BSON\MinKey {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyMinKey may not inherit from final class (%SBSON\MinKey) in %s on line %d

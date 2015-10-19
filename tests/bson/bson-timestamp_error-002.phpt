--TEST--
BSON BSON\Timestamp cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyTimestamp extends BSON\Timestamp {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyTimestamp may not inherit from final class (%SBSON\Timestamp) in %s on line %d

--TEST--
BSON BSON\ObjectID cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyObjectID extends BSON\ObjectID {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyObjectID may not inherit from final class (%SBSON\ObjectID) in %s on line %d

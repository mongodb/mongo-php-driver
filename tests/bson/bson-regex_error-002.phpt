--TEST--
BSON BSON\Regex cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyRegex extends BSON\Regex {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyRegex may not inherit from final class (%SBSON\Regex) in %s on line %d

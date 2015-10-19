--TEST--
BSON BSON\Javascript cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyJavascript extends BSON\Javascript {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyJavascript may not inherit from final class (%SBSON\Javascript) in %s on line %d

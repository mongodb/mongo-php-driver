--TEST--
BSON BSON\UTCDateTime cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

class MyUTCDateTime extends BSON\UTCDateTime {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyUTCDateTime may not inherit from final class (%SBSON\UTCDateTime) in %s on line %d

--TEST--
MongoDB\Driver\BulkWrite cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyBulkWrite extends MongoDB\Driver\BulkWrite {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyBulkWrite may not inherit from final class (MongoDB\Driver\BulkWrite) in %s on line %d

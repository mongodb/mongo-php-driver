--TEST--
MongoDB\Driver\WriteConcern cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyWriteConcern extends MongoDB\Driver\WriteConcern {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyWriteConcern may not inherit from final class (MongoDB\Driver\WriteConcern) in %s on line %d

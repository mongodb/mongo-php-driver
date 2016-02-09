--TEST--
MongoDB\Driver\ReadConcern cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyReadConcern extends MongoDB\Driver\ReadConcern {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyReadConcern may not inherit from final class (MongoDB\Driver\ReadConcern) in %s on line %d

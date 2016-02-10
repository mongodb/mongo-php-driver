--TEST--
MongoDB\Driver\CursorId cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyCursorId extends MongoDB\Driver\CursorId {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCursorId may not inherit from final class (MongoDB\Driver\CursorId) in %s on line %d

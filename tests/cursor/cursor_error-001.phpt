--TEST--
MongoDB\Driver\Cursor cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyCursor extends MongoDB\Driver\Cursor {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyCursor may not inherit from final class (MongoDB\Driver\Cursor) in %s on line %d

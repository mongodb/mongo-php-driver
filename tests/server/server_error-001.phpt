--TEST--
MongoDB\Driver\Server cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyServer extends MongoDB\Driver\Server {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyServer may not inherit from final class (MongoDB\Driver\Server) in %s on line %d

--TEST--
MongoDB\Driver\ReadPreference cannot be extended
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

class MyReadPreference extends MongoDB\Driver\ReadPreference {}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Fatal error: Class MyReadPreference may not inherit from final class (MongoDB\Driver\ReadPreference) in %s on line %d

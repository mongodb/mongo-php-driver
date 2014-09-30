--TEST--
MongoDB\Manager::__construct()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Manager(MONGODB_URI);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===

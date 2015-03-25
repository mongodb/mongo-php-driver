--TEST--
MongoDB\Driver\Manager::__construct()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===

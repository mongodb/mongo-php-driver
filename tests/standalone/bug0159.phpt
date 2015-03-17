--TEST--
PHPC-159: Memleak on failed path resolving
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$uri = parse_url(STANDALONE);
$manager = new MongoDB\Driver\Manager("mongodb:///tmp/mongodb-27018.sock,{$uri["host"]}:{$uri["port"]}");
$manager->executeQuery("foo.bar", new MongoDB\Driver\Query([]));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===


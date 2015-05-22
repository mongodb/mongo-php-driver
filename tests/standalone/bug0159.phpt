--TEST--
PHPC-159: Memleak on failed path resolving
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$uri = parse_url(STANDALONE);
$manager = new MongoDB\Driver\Manager("mongodb:///tmp/mongodb-27018.sock,{$uri["host"]}:{$uri["port"]}/?serverselectiontimeoutms=10");
try {
    $manager->executeQuery("foo.bar", new MongoDB\Driver\Query(array()));
} catch(Exception $e) {}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===


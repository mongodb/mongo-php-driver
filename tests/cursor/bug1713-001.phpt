--TEST--
PHPC-1713: MongoDB\Driver\Cursor::current() does not return anything
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));

var_dump($cursor->valid());
var_dump($cursor->current());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(false)
NULL
===DONE===

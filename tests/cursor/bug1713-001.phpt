--TEST--
MongoDB\Driver\Cursor PHPC-171: The cursor current method does not return anything
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

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

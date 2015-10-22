--TEST--
MongoDB\Driver\Manager: Executing Query without assignment still executes
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM uses HHVM's logging functionality"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));
$bulk->insert(array('_id' => 2, 'x' => 1));
$manager->executeBulkWrite(NS, $bulk);

ini_set("mongodb.debug", "stdout");
$manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)));
ini_set("mongodb.debug", "off");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
%a
[%s]     cursor: TRACE   > ENTRY: mongoc_cursor_next():%d
%a
[%s]     cursor: TRACE   >  EXIT: mongoc_cursor_destroy():%d
===DONE===

--TEST--
MongoDB\Driver\Session::getLogicalSessionId()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$session = $manager->startSession();

$lsid = $session->getLogicalSessionId();

/* Note: we avoid dumping the Binary object as it may contain bytes that
 * intefere with the test suite's ability to compare expected output. */
var_dump($lsid instanceof stdClass);
var_dump($lsid->id instanceof MongoDB\BSON\Binary);
var_dump($lsid->id->getType() === MongoDB\BSON\Binary::TYPE_UUID);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
===DONE===

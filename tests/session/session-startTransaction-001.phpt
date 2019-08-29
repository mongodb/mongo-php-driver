--TEST--
MongoDB\Driver\Session::startTransaction() ensure that methods can be called
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_no_transactions(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$session = $manager->startSession();

$session->startTransaction();
$session->abortTransaction();

$session->startTransaction();
$session->commitTransaction();

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
===DONE===

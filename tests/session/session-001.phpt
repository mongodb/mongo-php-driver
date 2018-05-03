--TEST--
MongoDB\Driver\Session spec test: Pool is LIFO
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$firstSession = $manager->startSession();
$firstSessionId = $firstSession->getLogicalSessionId();

unset($firstSession);

$secondSession = $manager->startSession();
$secondSessionId = $secondSession->getLogicalSessionId();

var_dump($firstSessionId == $secondSessionId);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===

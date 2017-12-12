--TEST--
MongoDB\Driver\Session spec test: Pool is LIFO
--SKIPIF--
<?php if (getenv("TRAVIS")) exit("skip This currently fails on Travis because it doesn't run 3.6 yet"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

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

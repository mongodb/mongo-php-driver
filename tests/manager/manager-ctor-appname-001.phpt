--TEST--
MongoDB\Driver\Manager::__construct(): appname option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$name1 = str_repeat('PHONGO', 20);
$name2 = str_repeat('PHONGO', 20);
$manager = new \MongoDB\Driver\Manager(STANDALONE . "/?appname={$name1}", ['appname' => "2-{$name2}"]);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand("test", $command);

?>
===DONE===
--EXPECT--
===DONE===

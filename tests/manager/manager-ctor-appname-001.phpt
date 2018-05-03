--TEST--
MongoDB\Driver\Manager::__construct(): appname option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$name1 = str_repeat('PHONGO', 20);
$name2 = str_repeat('PHONGO', 20);
$uri = append_uri_option(URI, "appname={$name1}");
$manager = new \MongoDB\Driver\Manager($uri, ['appname' => "2-{$name2}"]);

$command = new MongoDB\Driver\Command(['ping' => 1]);
$manager->executeCommand("test", $command);

?>
===DONE===
--EXPECT--
===DONE===

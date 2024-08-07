--TEST--
MongoDB\Driver\Server::getTags() with standalone
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_standalone(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

var_dump($manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::PRIMARY))->getTags());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(0) {
}
===DONE===

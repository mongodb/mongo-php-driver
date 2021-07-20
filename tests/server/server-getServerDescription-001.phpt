--TEST--
MongoDB\Driver\Server::getServerDescription()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

var_dump($manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY))->getServerDescription());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(%d) {
  %a
}
===DONE===

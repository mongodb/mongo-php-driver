--TEST--
MongoDB\Driver\Server::getInfo()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

try{
var_dump($manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY))->getInfo());
} catch (Exception $e) {}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(%d) {
  %a
}
===DONE===

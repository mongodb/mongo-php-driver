--TEST--
MongoDB\Driver\Server::getTags() with replica set
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

var_dump($manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY))->getTags());
var_dump($manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY))->getTags());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  ["dc"]=>
  string(2) "pa"
  ["ordinal"]=>
  string(3) "one"
}
array(2) {
  ["dc"]=>
  string(3) "nyc"
  ["ordinal"]=>
  string(3) "two"
}
===DONE===

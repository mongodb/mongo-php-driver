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
object(MongoDB\Driver\ServerDescription)#%d (%d) {
  ["host"]=>
  string(%d) "%s"
  ["port"]=>
  int(%d)
  ["type"]=>
  string(%d) "%r(Standalone|Mongos|RSPrimary)%r"
  ["hello_response"]=>
  array(%d) {
    %a
  }
  ["last_update_time"]=>
  int(%d)
  ["round_trip_time"]=>
  int(%d)
}
===DONE===

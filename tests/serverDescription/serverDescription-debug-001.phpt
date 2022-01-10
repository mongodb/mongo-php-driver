--TEST--
MongoDB\Driver\ServerDescription debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));
var_dump($server->getServerDescription());

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
  string(%d) "%r(Standalone|Mongos|RSPrimary|LoadBalancer)%r"
  ["hello_response"]=>
  array(%d) {%A
  }
  ["last_update_time"]=>
  %r(string\(\d+\) "\d+"|int\(\d+\))%r
  ["round_trip_time"]=>
  %r(NULL|int\(\d+\))%r
}
===DONE===

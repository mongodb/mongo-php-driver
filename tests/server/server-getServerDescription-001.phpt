--TEST--
MongoDB\Driver\Server::getServerDescription()
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
  int(%d)
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

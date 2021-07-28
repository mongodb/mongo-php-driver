--TEST--
MongoDB\Driver\ServerDescription debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
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

--TEST--
MongoDB\Driver\ServerDescription: var_export()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference('primary'));

echo var_export($server->getServerDescription(), true), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\ServerDescription::__set_state(array(
   'host' => '%s',
   'port' => %d,
   'type' => '%r(Standalone|Mongos|RSPrimary)%r',
   'hello_response' => 
  array (
    %a
  ),
   'last_update_time' => %d,
   'round_trip_time' => %d,
))
===DONE===

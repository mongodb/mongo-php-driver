--TEST--
MongoDB\Driver\Server debug output
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()))->getServer();

var_dump($server);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Server)#%d (%d) {
  ["host"]=>
  string(%d) "%s"
  ["port"]=>
  int(%d)
  ["type"]=>
  int(%d)
  ["is_primary"]=>
  bool(%s)
  ["is_secondary"]=>
  bool(%s)
  ["is_arbiter"]=>
  bool(false)
  ["is_hidden"]=>
  bool(false)
  ["is_passive"]=>
  bool(false)%A
  ["last_hello_response"]=>
  array(%d) {
    %a
  }
  ["round_trip_time"]=>
  %r(NULL|int\(\d+\))%r
}
===DONE===

--TEST--
MongoDB\Driver\Manager::getServers()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
<?php skip_if_no_arbiter(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);


$doc = array("example" => "document");
$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert($doc);
$wresult = $manager->executeBulkWrite(NS, $bulk);

var_dump($manager->getServers());
$servers = $manager->getServers();

foreach($servers as $server) {
    printf("%s:%d - primary: %d, secondary: %d, arbiter: %d\n", $server->getHost(), $server->getPort(), $server->isPrimary(), $server->isSecondary(), $server->isArbiter());
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(3) {
  [0]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    ["host"]=>
    string(%d) "%s"
    ["port"]=>
    int(%d)
    ["type"]=>
    int(4)
    ["is_primary"]=>
    bool(true)
    ["is_secondary"]=>
    bool(false)
    ["is_arbiter"]=>
    bool(false)
    ["is_hidden"]=>
    bool(false)
    ["is_passive"]=>
    bool(false)%A
    ["last_is_master"]=>
    array(%d) {
      %a
    }
    ["round_trip_time"]=>
    int(%d)
  }
  [1]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    ["host"]=>
    string(%d) "%s"
    ["port"]=>
    int(%d)
    ["type"]=>
    int(5)
    ["is_primary"]=>
    bool(false)
    ["is_secondary"]=>
    bool(true)
    ["is_arbiter"]=>
    bool(false)
    ["is_hidden"]=>
    bool(false)
    ["is_passive"]=>
    bool(false)%A
    ["last_is_master"]=>
    array(%d) {
      %a
    }
    ["round_trip_time"]=>
    int(%d)
  }
  [2]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    ["host"]=>
    string(%d) "%s"
    ["port"]=>
    int(%d)
    ["type"]=>
    int(6)
    ["is_primary"]=>
    bool(false)
    ["is_secondary"]=>
    bool(false)
    ["is_arbiter"]=>
    bool(true)
    ["is_hidden"]=>
    bool(false)
    ["is_passive"]=>
    bool(false)
    ["last_is_master"]=>
    array(%d) {
      %a
    }
    ["round_trip_time"]=>
    int(%d)
  }
}
%s:%d - primary: 1, secondary: 0, arbiter: 0
%s:%d - primary: 0, secondary: 1, arbiter: 0
%s:%d - primary: 0, secondary: 0, arbiter: 1
===DONE===

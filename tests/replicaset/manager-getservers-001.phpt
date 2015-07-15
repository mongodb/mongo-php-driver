--TEST--
MongoDB\Driver\Manager: getServers()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("REPLICASET"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);


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
    string(14) "192.168.112.10"
    ["port"]=>
    int(3000)
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
    bool(false)
    ["tags"]=>
    array(%d) {
      ["dc"]=>
      string(2) "pa"
      ["ordinal"]=>
      string(3) "one"
    }
    ["last_is_master"]=>
    array(17) {
      ["setName"]=>
      string(10) "REPLICASET"
      ["setVersion"]=>
      int(1)
      ["ismaster"]=>
      bool(true)
      ["secondary"]=>
      bool(false)
      ["hosts"]=>
      array(2) {
        [0]=>
        string(19) "192.168.112.10:3000"
        [1]=>
        string(19) "192.168.112.10:3001"
      }
      ["arbiters"]=>
      array(1) {
        [0]=>
        string(19) "192.168.112.10:3002"
      }
      ["primary"]=>
      string(19) "192.168.112.10:3000"
      ["tags"]=>
      array(%d) {
        ["dc"]=>
        string(2) "pa"
        ["ordinal"]=>
        string(3) "one"
      }
      ["me"]=>
      string(19) "192.168.112.10:3000"
      ["electionId"]=>
      object(%s\ObjectID)#%d (1) {
        ["oid"]=>
        string(24) "%s"
      }
      ["maxBsonObjectSize"]=>
      int(16777216)
      ["maxMessageSizeBytes"]=>
      int(48000000)
      ["maxWriteBatchSize"]=>
      int(1000)
      ["localTime"]=>
      object(%s\UTCDateTime)#%d (%d) {
      }
      ["maxWireVersion"]=>
      int(3)
      ["minWireVersion"]=>
      int(0)
      ["ok"]=>
      float(1)
    }
    ["round_trip_time"]=>
    int(%d)
  }
  [1]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    ["host"]=>
    string(14) "192.168.112.10"
    ["port"]=>
    int(3001)
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
    bool(false)
    ["tags"]=>
    array(%d) {
      ["dc"]=>
      string(3) "nyc"
      ["ordinal"]=>
      string(3) "two"
    }
    ["last_is_master"]=>
    array(16) {
      ["setName"]=>
      string(10) "REPLICASET"
      ["setVersion"]=>
      int(1)
      ["ismaster"]=>
      bool(false)
      ["secondary"]=>
      bool(true)
      ["hosts"]=>
      array(2) {
        [0]=>
        string(19) "192.168.112.10:3000"
        [1]=>
        string(19) "192.168.112.10:3001"
      }
      ["arbiters"]=>
      array(1) {
        [0]=>
        string(19) "192.168.112.10:3002"
      }
      ["primary"]=>
      string(19) "192.168.112.10:3000"
      ["tags"]=>
      array(%d) {
        ["dc"]=>
        string(3) "nyc"
        ["ordinal"]=>
        string(3) "two"
      }
      ["me"]=>
      string(19) "192.168.112.10:3001"
      ["maxBsonObjectSize"]=>
      int(16777216)
      ["maxMessageSizeBytes"]=>
      int(48000000)
      ["maxWriteBatchSize"]=>
      int(1000)
      ["localTime"]=>
      object(%s\UTCDateTime)#%d (%d) {
      }
      ["maxWireVersion"]=>
      int(3)
      ["minWireVersion"]=>
      int(0)
      ["ok"]=>
      float(1)
    }
    ["round_trip_time"]=>
    int(%d)
  }
  [2]=>
  object(MongoDB\Driver\Server)#%d (%d) {
    ["host"]=>
    string(14) "192.168.112.10"
    ["port"]=>
    int(3002)
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
    array(16) {
      ["setName"]=>
      string(10) "REPLICASET"
      ["setVersion"]=>
      int(1)
      ["ismaster"]=>
      bool(false)
      ["secondary"]=>
      bool(false)
      ["hosts"]=>
      array(2) {
        [0]=>
        string(19) "192.168.112.10:3000"
        [1]=>
        string(19) "192.168.112.10:3001"
      }
      ["arbiters"]=>
      array(1) {
        [0]=>
        string(19) "192.168.112.10:3002"
      }
      ["primary"]=>
      string(19) "192.168.112.10:3000"
      ["arbiterOnly"]=>
      bool(true)
      ["me"]=>
      string(19) "192.168.112.10:3002"
      ["maxBsonObjectSize"]=>
      int(16777216)
      ["maxMessageSizeBytes"]=>
      int(48000000)
      ["maxWriteBatchSize"]=>
      int(1000)
      ["localTime"]=>
      object(%s\UTCDateTime)#8 (0) {
      }
      ["maxWireVersion"]=>
      int(3)
      ["minWireVersion"]=>
      int(0)
      ["ok"]=>
      float(1)
    }
    ["round_trip_time"]=>
    int(%d)
  }
}
192.168.112.10:3000 - primary: 1, secondary: 0, arbiter: 0
192.168.112.10:3001 - primary: 0, secondary: 1, arbiter: 0
192.168.112.10:3002 - primary: 0, secondary: 0, arbiter: 1
===DONE===

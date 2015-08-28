--TEST--
MongoDB\Driver\Server debugInfo
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
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
  int(1)
  ["is_primary"]=>
  bool(false)
  ["is_secondary"]=>
  bool(false)
  ["is_arbiter"]=>
  bool(false)
  ["is_hidden"]=>
  bool(false)
  ["is_passive"]=>
  bool(false)
  ["last_is_master"]=>
  array(8) {
    ["ismaster"]=>
    bool(true)
    ["maxBsonObjectSize"]=>
    int(16777216)
    ["maxMessageSizeBytes"]=>
    int(48000000)
    ["maxWriteBatchSize"]=>
    int(1000)
    ["localTime"]=>
    object(%s\UTCDateTime)#%d (%d) {
      ["milliseconds"]=>
      %r(int\(\d+\)|string\(\d+\) "\d+")%r
    }
    ["maxWireVersion"]=>
    int(%d)
    ["minWireVersion"]=>
    int(0)
    ["ok"]=>
    float(1)
  }
  ["round_trip_time"]=>
  int(%d)
}
===DONE===

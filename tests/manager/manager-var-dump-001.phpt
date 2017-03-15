--TEST--
MongoDB\Driver\Manager: Constructing invalid manager
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('STANDALONE'); CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);
var_dump($manager);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(array("my" => "value"));

$retval = $manager->executeBulkWrite(NS, $bulk);
var_dump($manager);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(%d) "mongodb://%s"
  ["cluster"]=>
  array(0) {
  }
}
object(MongoDB\Driver\Manager)#%d (%d) {
  ["uri"]=>
  string(%d) "mongodb://%s"
  ["cluster"]=>
  array(1) {
    [0]=>
    array(10) {
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
      array(%d) {
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
        %a
      }
      ["round_trip_time"]=>
      int(%d)
    }
  }
}
===DONE===

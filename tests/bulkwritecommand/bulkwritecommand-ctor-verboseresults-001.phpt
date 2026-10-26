--TEST--
MongoDB\Driver\BulkWriteCommand::__construct() verboseResults=true
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$bulk = new MongoDB\Driver\BulkWriteCommand(['verboseResults' => true]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->updateOne(NS, ['_id' => 1], ['$set' => ['x' => 1]]);
$bulk->deleteOne(NS, ['_id' => 1]);

$result = $manager->executeBulkWriteCommand($bulk);

var_dump($result);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWriteCommandResult)#%d (%d) {
  ["isAcknowledged"]=>
  bool(true)
  ["insertedCount"]=>
  int(1)
  ["matchedCount"]=>
  int(1)
  ["modifiedCount"]=>
  int(1)
  ["upsertedCount"]=>
  int(0)
  ["deletedCount"]=>
  int(1)
  ["insertResults"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(40) "HQAAAAMwABUAAAAQaW5zZXJ0ZWRJZAABAAAAAAA="
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["0"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(28) "FQAAABBpbnNlcnRlZElkAAEAAAAA"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["insertedId"]=>
          int(1)
        }
      }
    }
  }
  ["updateResults"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(80) "OgAAAAMxADIAAAASbWF0Y2hlZENvdW50AAEAAAAAAAAAEm1vZGlmaWVkQ291bnQAAQAAAAAAAAAAAA=="
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["1"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(68) "MgAAABJtYXRjaGVkQ291bnQAAQAAAAAAAAASbW9kaWZpZWRDb3VudAABAAAAAAAAAAA="
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["matchedCount"]=>
          int(1)
          ["modifiedCount"]=>
          int(1)
        }
      }
    }
  }
  ["deleteResults"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(48) "IwAAAAMyABsAAAASZGVsZXRlZENvdW50AAEAAAAAAAAAAAA="
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["2"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(36) "GwAAABJkZWxldGVkQ291bnQAAQAAAAAAAAAA"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["deletedCount"]=>
          int(1)
        }
      }
    }
  }
}
===DONE===

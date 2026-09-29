--TEST--
MongoDB\Driver\Server::executeBulkWriteCommand()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$server = $manager->selectServer();

$bulk = new MongoDB\Driver\BulkWriteCommand(['verboseResults' => true]);
$bulk->insertOne(NS, ['_id' => 1]);
$bulk->insertOne(NS, ['_id' => 2]);
$bulk->insertOne(NS, ['_id' => 3]);
$bulk->insertOne(NS, ['_id' => 4]);
$bulk->insertOne(NS, ['_id' => 5]);
$bulk->replaceOne(NS, ['_id' => 1], ['x' => 1]);
$bulk->updateOne(NS, ['_id' => 2], ['$set' => ['x' => 1]]);
$bulk->updateMany(NS, ['x' => ['$exists' => true]], ['$inc' => ['x' => 1]]);
$bulk->deleteOne(NS, ['_id' => ['$gt' => 4]]);
$bulk->deleteMany(NS, ['_id' => ['$gt' => 2]]);

$result = $server->executeBulkWriteCommand($bulk);

var_dump($result);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\BulkWriteCommandResult)#%d (%d) {
  ["isAcknowledged"]=>
  bool(true)
  ["insertedCount"]=>
  int(5)
  ["matchedCount"]=>
  int(4)
  ["modifiedCount"]=>
  int(4)
  ["upsertedCount"]=>
  int(0)
  ["deletedCount"]=>
  int(3)
  ["insertResults"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(168) "%s"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["0"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(28) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["insertedId"]=>
          int(1)
        }
      }
      ["1"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(28) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["insertedId"]=>
          int(2)
        }
      }
      ["2"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(28) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["insertedId"]=>
          int(3)
        }
      }
      ["3"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(28) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["insertedId"]=>
          int(4)
        }
      }
      ["4"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(28) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["insertedId"]=>
          int(5)
        }
      }
    }
  }
  ["updateResults"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(220) "%s"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["5"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(68) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["matchedCount"]=>
          int(1)
          ["modifiedCount"]=>
          int(1)
        }
      }
      ["6"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(68) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["matchedCount"]=>
          int(1)
          ["modifiedCount"]=>
          int(1)
        }
      }
      ["7"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(68) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["matchedCount"]=>
          int(2)
          ["modifiedCount"]=>
          int(2)
        }
      }
    }
  }
  ["deleteResults"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(88) "%s"
    ["value"]=>
    object(stdClass)#%d (%d) {
      ["8"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(36) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["deletedCount"]=>
          int(1)
        }
      }
      ["9"]=>
      object(MongoDB\BSON\Document)#%d (%d) {
        ["data"]=>
        string(36) "%s"
        ["value"]=>
        object(stdClass)#%d (%d) {
          ["deletedCount"]=>
          int(2)
        }
      }
    }
  }
}
===DONE===

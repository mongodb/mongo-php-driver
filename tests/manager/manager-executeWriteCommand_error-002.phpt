--TEST--
MongoDB\Driver\Manager::executeWriteCommand() throws CommandException for invalid writeConcern
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET'); CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);

$command = new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME,
    'query' => ['_id' => 'foo'],
    'update' => ['foo' => ['bar']],
    'upsert' => true,
    'new' => true,
]);

try {
    $manager->executeWriteCommand(DATABASE_NAME, $command, ['writeConcern' => new MongoDB\Driver\WriteConcern("undefined")]);
} catch (MongoDB\Driver\Exception\CommandException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
    var_dump($e->getResultDocument());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\Exception\CommandException(79): Write Concern error: No write concern mode named 'undefined' found in replica set configuration
object(stdClass)#%d (%d) {
  ["lastErrorObject"]=>
  object(stdClass)#%d (3) {
    ["n"]=>
    int(1)
    ["updatedExisting"]=>
    bool(false)
    ["upserted"]=>
    string(3) "foo"
  }
  ["value"]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    string(3) "foo"
    ["foo"]=>
    array(1) {
      [0]=>
      string(3) "bar"
    }
  }
  ["writeConcernError"]=>
  object(stdClass)#%d (3) {
    ["code"]=>
    int(79)
    ["codeName"]=>
    string(23) "UnknownReplWriteConcern"
    ["errmsg"]=>
    string(74) "No write concern mode named 'undefined' found in replica set configuration"
  }
  ["ok"]=>
  float(1)%A
}
===DONE===

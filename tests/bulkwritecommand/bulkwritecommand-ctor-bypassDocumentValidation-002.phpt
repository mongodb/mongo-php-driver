--TEST--
MongoDB\Driver\BulkWriteCommand::__construct() bypassDocumentValidation=false
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '8.0'); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$manager->executeWriteCommand(DATABASE_NAME, new MongoDB\Driver\Command([
    'create' => COLLECTION_NAME,
    'validator' => [
        '$jsonSchema' => [
            'bsonType' => 'object',
            'required' => ['x'],
        ],
    ],
]));

$bulk = new MongoDB\Driver\BulkWriteCommand(['bypassDocumentValidation' => false]);
/* Include a successful write operation to ensure that mongoc_bulkwriteresult_t
 * is populated (CDRIVER-5856). */
$bulk->insertOne(NS, ['_id' => 1, 'x' => 1]);
$bulk->insertOne(NS, ['_id' => 2]);

try {
    $manager->executeBulkWriteCommand($bulk);
} catch (MongoDB\Driver\Exception\BulkWriteCommandException $e) {
    printf("%s(%d): %s\n", get_class($e), $e->getCode(), $e->getMessage());
    var_dump($e->getPartialResult());
    var_dump($e->getWriteErrors());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
MongoDB\Driver\Exception\BulkWriteCommandException(0): Bulk write failed
object(MongoDB\Driver\BulkWriteCommandResult)#%d (%d) {
  ["isAcknowledged"]=>
  bool(true)
  ["insertedCount"]=>
  int(1)
  ["matchedCount"]=>
  int(0)
  ["modifiedCount"]=>
  int(0)
  ["upsertedCount"]=>
  int(0)
  ["deletedCount"]=>
  int(0)
  ["insertResults"]=>
  NULL
  ["updateResults"]=>
  NULL
  ["deleteResults"]=>
  NULL
}
array(1) {
  [1]=>
  object(MongoDB\Driver\WriteError)#%d (%d) {
    ["message"]=>
    string(26) "Document failed validation"
    ["code"]=>
    int(121)
    ["index"]=>
    int(1)
    ["info"]=>
    object(stdClass)#%d (%d) {
      ["failingDocumentId"]=>
      int(2)
      ["details"]=>
      object(stdClass)#%d (%d) {
        ["operatorName"]=>
        string(11) "$jsonSchema"
        ["schemaRulesNotSatisfied"]=>
        array(1) {
          [0]=>
          object(stdClass)#%d (%d) {
            ["operatorName"]=>
            string(8) "required"
            ["specifiedAs"]=>
            object(stdClass)#%d (%d) {
              ["required"]=>
              array(1) {
                [0]=>
                string(1) "x"
              }
            }
            ["missingProperties"]=>
            array(1) {
              [0]=>
              string(1) "x"
            }
          }
        }
      }
    }
  }
}
===DONE===

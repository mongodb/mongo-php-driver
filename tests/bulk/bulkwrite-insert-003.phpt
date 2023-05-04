--TEST--
MongoDB\Driver\BulkWrite::insert() appends ID when inserting Document instances
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

$document = MongoDB\BSON\Document::fromPHP(['foo' => 'bar']);
$array = MongoDB\BSON\PackedArray::fromPHP(['bar']);

$bulk = new MongoDB\Driver\BulkWrite();

$insertedDocumentId = $bulk->insert($document);
$insertedArrayId = $bulk->insert($array);

$result = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

var_dump($insertedDocumentId);
var_dump($document->toPHP());

var_dump($insertedArrayId);
var_dump($array->toPHP());

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump($cursor->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 2 document(s)
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
object(stdClass)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
array(1) {
  [0]=>
  string(3) "bar"
}
array(2) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
    ["foo"]=>
    string(3) "bar"
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    object(MongoDB\BSON\ObjectId)#%d (%d) {
      ["oid"]=>
      string(24) "%x"
    }
    ["0"]=>
    string(3) "bar"
  }
}
===DONE===

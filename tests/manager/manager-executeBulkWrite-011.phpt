--TEST--
MongoDB\Driver\BulkWrite: bypassDocumentValidation option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_server_version('<', '3.2'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$command = new MongoDB\Driver\Command([
    'create' => COLLECTION_NAME,
    'validator' => ['x' => ['$type' => 'number']],
]);
$manager->executeCommand(DATABASE_NAME, $command);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1, 'x' => 1]);
$bulk->insert(['_id' => 2, 'x' => 2]);
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite(['bypassDocumentValidation' => true]);
$bulk->update(['_id' => 2], ['$set' => ['x' => 'two']]);
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite(['bypassDocumentValidation' => true]);
$bulk->insert(['_id' => 3, 'x' => 'three']);
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 4, 'x' => 'four']);

echo throws(function() use($manager, $bulk) {
    $manager->executeBulkWrite(NS, $bulk);
}, "MongoDB\Driver\Exception\BulkWriteException"), "\n";

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update(['_id' => 1], ['$set' => ['x' => 'one']]);

echo throws(function() use($manager, $bulk) {
    $manager->executeBulkWrite(NS, $bulk);
}, "MongoDB\Driver\Exception\BulkWriteException"), "\n";

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update(['_id' => 2], ['$set' => ['x' => 2]]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\BulkWriteException
Document failed validation
OK: Got MongoDB\Driver\Exception\BulkWriteException
Document failed validation
array(3) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(1)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(2)
  }
  [2]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    string(5) "three"
  }
}
===DONE===

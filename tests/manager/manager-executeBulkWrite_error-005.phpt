--TEST--
MongoDB\Driver\Manager::executeBulkWrite() insert write error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['$foo' => 1]);

try {
    $manager->executeBulkWrite(NS, $bulk);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    printf("BulkWriteException: %s\n", $e->getMessage());

    echo "\n===> WriteResult\n";
    printWriteResult($e->getWriteResult());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
BulkWriteException: BulkWrite error

===> WriteResult
server: %s:%d
insertedCount: 0
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0
object(MongoDB\Driver\WriteError)#%d (%d) {
  ["message"]=>
  string(48) "Document can't have $ prefixed field names: $foo"
  ["code"]=>
  int(2)
  ["index"]=>
  int(0)
  ["info"]=>
  NULL
}
writeError[0].message: Document can't have $ prefixed field names: $foo
writeError[0].code: 2
===DONE===

--TEST--
MongoDB\Driver\Manager::executeBulkWrite() update write error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('x' => 1));
$manager->executeBulkWrite(NS, $bulk);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->update(['x' => 1], ['$foo' => 1]);

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
BulkWriteException: Unknown modifier: $foo%S

===> WriteResult
server: %s:%d
insertedCount: 0
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0
object(MongoDB\Driver\WriteError)#%d (%d) {
  ["message"]=>
  string(%d) "Unknown modifier: $foo%S"
  ["code"]=>
  int(9)
  ["index"]=>
  int(0)
  ["info"]=>
  NULL
}
writeError[0].message: Unknown modifier: $foo%S
writeError[0].code: 9
===DONE===

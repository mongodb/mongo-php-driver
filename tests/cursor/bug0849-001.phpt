--TEST--
PHPC-849: Cursor::setTypeMap() leaks current element if called during iteration
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
$cursor->setTypeMap(['root' => 'stdClass']);

foreach ($cursor as $i => $document) {
    // Type map will apply to the next iteration, since current element is already converted
    $cursor->setTypeMap(['root' => ($i % 2 ? 'stdClass' : 'array')]);
    var_dump($document);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(1)
}
array(1) {
  ["_id"]=>
  int(2)
}
object(stdClass)#%d (%d) {
  ["_id"]=>
  int(3)
}
===DONE===

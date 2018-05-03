--TEST--
MongoDB\Driver\BulkWrite cannot be executed multiple times
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$result = $manager->executeBulkWrite(NS, $bulk);
printf("Inserted %d document(s)\n", $result->getInsertedCount());

echo throws(function() use ($manager, $bulk) {
    $result = $manager->executeBulkWrite(NS, $bulk);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Inserted 1 document(s)
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
BulkWrite objects may only be executed once and this instance has already been executed
===DONE===

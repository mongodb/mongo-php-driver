--TEST--
MongoDB\Driver\Manager::executeBulkWrite() rejects a null byte in the namespace
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

/* A null byte in the collection name would truncate the namespace at the
 * C-string layer and silently retarget the write. */
echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite("database.col\0lection", $bulk);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* A null byte in the database name is rejected the same way. */
echo throws(function() use ($manager) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(['x' => 1]);
    $manager->executeBulkWrite("data\0base.collection", $bulk);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid namespace provided: namespaces may not contain a null byte
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid namespace provided: namespaces may not contain a null byte
===DONE===

--TEST--
MongoDB\Driver\Manager::executeQuery() rejects a null byte in the namespace
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();

/* A null byte in the collection name would truncate the namespace at the
 * C-string layer and silently retarget the query. */
echo throws(function() use ($manager) {
    $manager->executeQuery("database.col\0lection", new MongoDB\Driver\Query([]));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* A null byte in the database name is rejected the same way. */
echo throws(function() use ($manager) {
    $manager->executeQuery("data\0base.collection", new MongoDB\Driver\Query([]));
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

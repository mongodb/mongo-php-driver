--TEST--
MongoDB\Driver\Manager::executeDelete() write error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, ['x' => 1]);

echo throws(function() use ($manager) {
    $manager->executeDelete(NS, ['$foo' => 1], ['limit' => 1]);
}, 'MongoDB\Driver\Exception\WriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\WriteException
unknown top level operator: $foo
===DONE===

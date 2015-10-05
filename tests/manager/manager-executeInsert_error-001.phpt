--TEST--
MongoDB\Driver\Manager::executeInsert() write error
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

echo throws(function() use ($manager) {
    $manager->executeInsert(NS, ['$foo' => 1]);
}, 'MongoDB\Driver\Exception\WriteException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\WriteException
Document can't have $ prefixed field names: $foo
===DONE===

--TEST--
PHPC-924: Using a projection with an empty field name causes a crash when destroying cursor
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$query = new MongoDB\Driver\Query([], ['projection' => ['' => 1]]);

echo throws(function() use ($manager, $query) {
    $cursor = $manager->executeQuery(NS, $query);
    var_dump($cursor->toArray());
}, 'MongoDB\Driver\Exception\RuntimeException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\RuntimeException
Cannot use empty keys in 'opts'.
===DONE===

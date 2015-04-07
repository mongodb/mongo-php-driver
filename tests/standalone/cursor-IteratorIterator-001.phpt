--TEST--
MongoDB\Driver\Cursor query result iteration through IteratorIterator
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 2, 'x' => 1));

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)));

foreach (new IteratorIterator($cursor) as $document) {
    var_dump($document);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(2) {
  ["_id"]=>
  int(1)
  ["x"]=>
  int(1)
}
array(2) {
  ["_id"]=>
  int(2)
  ["x"]=>
  int(1)
}
===DONE===

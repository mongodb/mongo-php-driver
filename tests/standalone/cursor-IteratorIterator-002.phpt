--TEST--
MongoDB\Driver\Cursor command result iteration through IteratorIterator
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$manager->executeInsert(NS, array('_id' => 1, 'x' => 1));
$manager->executeInsert(NS, array('_id' => 2, 'x' => 1));

$command = new MongoDB\Driver\Command(array(
    'aggregate' => COLLECTION_NAME,
    'pipeline' => array(
        array('$match' => array('x' => 1)),
    ),
    'cursor' => new stdClass,
));

$cursor = $manager->executeCommand(DATABASE_NAME, $command);

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

--TEST--
MongoDB\Driver\Result::getIterator() command result iteration
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

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

echo "Traversing Result:\n";

$result = $manager->executeCommand(DATABASE_NAME, $command);

foreach ($result as $document) {
    var_dump($document);
}

echo "Traversing Result::getIterator():\n";

$result = $manager->executeQuery(NS, $query);

foreach ($result->getIterator() as $document) {
    var_dump($document);
}

echo "\nTraversing IteratorIterator constructed from Result:\n";

$result = $manager->executeCommand(DATABASE_NAME, $command);

foreach (new IteratorIterator($result) as $document) {
    var_dump($document);
}

echo "\nTraversing IteratorIterator constructed from Result::getIterator():\n";

$result = $manager->executeCommand(DATABASE_NAME, $command);

foreach (new IteratorIterator($result->getIterator()) as $document) {
    var_dump($document);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Traversing Result:
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

Traversing Result::getIterator():
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

Traversing IteratorIterator constructed from Result:
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

Traversing IteratorIterator constructed from Result::getIterator():
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

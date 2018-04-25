--TEST--
PHPC-243: Manager::executeUpdate() & Bulk->update() w/o multi
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

// load fixtures for test
$bulk = new \MongoDB\Driver\BulkWrite;
$bulk->insert(array('_id' => 1, 'x' => 1));
$bulk->insert(array('_id' => 2, 'x' => 2));
$bulk->insert(array('_id' => 3, 'x' => 2));
$bulk->insert(array('_id' => 4, 'x' => 2));
$bulk->insert(array('_id' => 5, 'x' => 1));
$bulk->insert(array('_id' => 6, 'x' => 1));
$manager->executeBulkWrite(NS, $bulk);

$bulk = new \MongoDB\Driver\BulkWrite;
$bulk->update(
    array('x' => 1),
    array('$set' => array('x' => 3)),
    array('multi' => false, 'upsert' => false)
);
$result = $manager->executeBulkWrite(NS, $bulk);

printf("Changed %d out of expected 1 (_id=1)\n", $result->getModifiedCount());

$bulk = new \MongoDB\Driver\BulkWrite;
$bulk->update(
    array('x' => 1),
    array('$set' => array('x' => 2)),
    array('multi' => true, 'upsert' => false)
);
$result = $manager->executeBulkWrite(NS, $bulk);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
var_dump(iterator_to_array($cursor));
printf("Changed %d out of expected 2, (_id=5, _id=6)\n", $result->getModifiedCount());

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(
    array('x' => 2),
    array('$set' => array('x' => 4)),
    array('multi' => false, 'upsert' => false)
);

$result = $manager->executeBulkWrite(NS, $bulk);
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
var_dump(iterator_to_array($cursor));
printf("Changed %d out of expected 1, (_id=2)\n", $result->getModifiedCount());



$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(
    array('x' => 2),
    array('$set' => array('x' => 41)),
    array('multi' => false, 'upsert' => false)
);

$result = $manager->executeBulkWrite(NS, $bulk);
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
var_dump(iterator_to_array($cursor));
printf("Changed %d out of expected 1 (id_=3)\n", $result->getModifiedCount());



$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update(
    array('x' => 2),
    array('$set' => array('x' => 42)),
    array('multi' => true, 'upsert' => false)
);

$result = $manager->executeBulkWrite(NS, $bulk);
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
var_dump(iterator_to_array($cursor));
printf("Changed %d out of expected 3 (_id=4, _id=5, _id=6)\n", $result->getModifiedCount());
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Changed 1 out of expected 1 (_id=1)
array(6) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(3)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(2)
  }
  [2]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(2)
  }
  [3]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(4)
    ["x"]=>
    int(2)
  }
  [4]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(5)
    ["x"]=>
    int(2)
  }
  [5]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(6)
    ["x"]=>
    int(2)
  }
}
Changed 2 out of expected 2, (_id=5, _id=6)
array(6) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(3)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(4)
  }
  [2]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(2)
  }
  [3]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(4)
    ["x"]=>
    int(2)
  }
  [4]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(5)
    ["x"]=>
    int(2)
  }
  [5]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(6)
    ["x"]=>
    int(2)
  }
}
Changed 1 out of expected 1, (_id=2)
array(6) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(3)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(4)
  }
  [2]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(41)
  }
  [3]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(4)
    ["x"]=>
    int(2)
  }
  [4]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(5)
    ["x"]=>
    int(2)
  }
  [5]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(6)
    ["x"]=>
    int(2)
  }
}
Changed 1 out of expected 1 (id_=3)
array(6) {
  [0]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(3)
  }
  [1]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(4)
  }
  [2]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(41)
  }
  [3]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(4)
    ["x"]=>
    int(42)
  }
  [4]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(5)
    ["x"]=>
    int(42)
  }
  [5]=>
  object(stdClass)#%d (2) {
    ["_id"]=>
    int(6)
    ["x"]=>
    int(42)
  }
}
Changed 3 out of expected 3 (_id=4, _id=5, _id=6)
===DONE===

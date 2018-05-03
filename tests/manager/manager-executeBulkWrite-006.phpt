--TEST--
MongoDB\Driver\Manager::executeBulkWrite() insert one document (with embedded)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";
require_once __DIR__ . "/../utils/classes.inc";

$manager = new MongoDB\Driver\Manager(URI);

$hannes = new Person("Hannes", 42);
$sunnyvale = new Address(94086, "USA");
$kopavogur = new Address(200, "Iceland");
$hannes->addAddress($sunnyvale);
$hannes->addAddress($kopavogur);

$mikola = new Person("Jeremy", 21);
$michigan = new Address(48169, "USA");

$hannes->addFriend($mikola);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert($hannes);
$result = $manager->executeBulkWrite(NS, $bulk);

echo "\n===> WriteResult\n";
printWriteResult($result);

echo "\n===> Collection\n";
$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()));
foreach($cursor as $object) {
    var_dump($object);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
===> WriteResult
server: %s:%d
insertedCount: 1
matchedCount: 0
modifiedCount: 0
upsertedCount: 0
deletedCount: 0

===> Collection
object(Person)#%d (5) {
  ["name":protected]=>
  string(6) "Hannes"
  ["age":protected]=>
  int(42)
  ["addresses":protected]=>
  array(2) {
    [0]=>
    object(Address)#%d (2) {
      ["zip":protected]=>
      int(94086)
      ["country":protected]=>
      string(3) "USA"
    }
    [1]=>
    object(Address)#%d (2) {
      ["zip":protected]=>
      int(200)
      ["country":protected]=>
      string(7) "Iceland"
    }
  }
  ["friends":protected]=>
  array(1) {
    [0]=>
    object(Person)#%d (5) {
      ["name":protected]=>
      string(6) "Jeremy"
      ["age":protected]=>
      int(21)
      ["addresses":protected]=>
      array(0) {
      }
      ["friends":protected]=>
      array(0) {
      }
      ["secret":protected]=>
      string(4) "none"
    }
  }
  ["secret":protected]=>
  string(4) "none"
}
===DONE===

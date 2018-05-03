--TEST--
_id should only be generated for top-level document, not embedded docs
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$user = array(
    "username" => "bob",
    "address" => array(
        "street" => "Main St.",
        "city" => "New York",
    ),
);

$bulk = new MongoDB\Driver\BulkWrite();
$user["_id"] = $bulk->insert($user);
$result = $manager->executeBulkWrite(NS, $bulk);

echo "Dumping inserted user document with injected _id:\n";
var_dump($user);

$cursor = $manager->executeQuery(NS, new MongoDB\Driver\Query(array("_id" => $user["_id"])));

echo "\nDumping fetched user document:\n";
$array = $cursor->toArray();
var_dump($array[0]);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Dumping inserted user document with injected _id:
array(3) {
  ["username"]=>
  string(3) "bob"
  ["address"]=>
  array(2) {
    ["street"]=>
    string(8) "Main St."
    ["city"]=>
    string(8) "New York"
  }
  ["_id"]=>
  object(%s\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%s"
  }
}

Dumping fetched user document:
object(stdClass)#%d (3) {
  ["_id"]=>
  object(%s\ObjectId)#%d (%d) {
    ["oid"]=>
    string(24) "%s"
  }
  ["username"]=>
  string(3) "bob"
  ["address"]=>
  object(stdClass)#%d (%d) {
    ["street"]=>
    string(8) "Main St."
    ["city"]=>
    string(8) "New York"
  }
}
===DONE===

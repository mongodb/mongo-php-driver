--TEST--
MongoDB\Driver\Server::executeBulkWrite()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()))->getServer();

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(array('_id' => 1, 'x' => 1));
$bulk->insert(array('_id' => 2, 'x' => 2));
$bulk->update(array('x' => 2), array('$set' => array('x' => 1)), array("limit" => 1, "upsert" => false));
$bulk->update(array('_id' => 3), array('$set' => array('x' => 3)), array("limit" => 1, "upsert" => true));
$bulk->delete(array('x' => 1), array("limit" => 1));

$result = $server->executeBulkWrite(NS, $bulk);

printf("WriteResult.server is the same: %s\n", $server == $result->getServer() ? 'yes' : 'no');

echo "\n===> WriteResult\n";
printWriteResult($result);
var_dump($result);

echo "\n===> Collection\n";
$cursor = $server->executeQuery(NS, new MongoDB\Driver\Query(array()));
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
WriteResult.server is the same: yes

===> WriteResult
server: %s:%d
insertedCount: 2
matchedCount: 1
modifiedCount: 1
upsertedCount: 1
deletedCount: 1
upsertedId[3]: int(3)
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["nInserted"]=>
  int(2)
  ["nMatched"]=>
  int(1)
  ["nModified"]=>
  int(1)
  ["nRemoved"]=>
  int(1)
  ["nUpserted"]=>
  int(1)
  ["upsertedIds"]=>
  array(1) {
    [0]=>
    array(%d) {
      ["index"]=>
      int(3)
      ["_id"]=>
      int(3)
    }
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (%d) {
  }
}

===> Collection
array(2) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(1)
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(3)
  }
}
===DONE===

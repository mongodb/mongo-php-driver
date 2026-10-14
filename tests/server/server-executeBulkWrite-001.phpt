--TEST--
MongoDB\Driver\Server::executeBulkWrite()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
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
  ["insertedCount"]=>
  int(2)
  ["matchedCount"]=>
  int(1)
  ["modifiedCount"]=>
  int(1)
  ["deletedCount"]=>
  int(1)
  ["upsertedCount"]=>
  int(1)
  ["server"]=>
  object(MongoDB\Driver\Server)#%d (%d) {%A
  }
  ["upsertedIds"]=>
  array(1) {
    [3]=>
    int(3)
  }
  ["writeErrors"]=>
  array(0) {
  }
  ["writeConcernError"]=>
  NULL
  ["writeConcern"]=>
  object(MongoDB\Driver\WriteConcern)#%d (3) {
    ["w"]=>
    NULL
    ["j"]=>
    NULL
    ["wtimeout"]=>
    int(0)
  }
  ["errorReplies"]=>
  array(0) {
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

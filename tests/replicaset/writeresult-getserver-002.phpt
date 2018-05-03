--TEST--
MongoDB\Driver\Server: Manager->getServer() returning correct server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_clean(); ?>
<?php skip_if_not_clean('local', 'examples'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);


$doc = array("example" => "document");
$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert($doc);
$wresult = $manager->executeBulkWrite(NS, $bulk);

$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert($doc);

/* writes go to the primary */
$server = $wresult->getServer();
/* This is the same server */
$server2 = $server->executeBulkWrite(NS, $bulk)->getServer();

/* Both are the primary, e.g. the same server */
var_dump($server == $server2);


$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY);
/* Fetch a secondary */
$server3 = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()), $rp)->getServer();

var_dump($server == $server3);
var_dump($server->getPort(), $server3->getPort());


$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert($doc);

$result = $server3->executeBulkWrite("local.examples", $bulk);
var_dump($result, $result->getServer()->getHost(), $result->getServer()->getPort());
$result = $server3->executeQuery("local.examples", new MongoDB\Driver\Query(array()));
foreach($result as $document) {
    var_dump($document);
}
$cmd = new MongoDB\Driver\Command(array("drop" => "examples"));
$server3->executeCommand("local", $cmd);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
int(3000)
int(3001)
object(MongoDB\Driver\WriteResult)#%d (%d) {
  ["nInserted"]=>
  int(1)
  ["nMatched"]=>
  int(0)
  ["nModified"]=>
  int(0)
  ["nRemoved"]=>
  int(0)
  ["nUpserted"]=>
  int(0)
  ["upsertedIds"]=>
  array(0) {
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
string(14) "192.168.112.10"
int(3001)
object(stdClass)#%d (2) {
  ["_id"]=>
  object(%s\ObjectId)#%d (1) {
    ["oid"]=>
    string(24) "%s"
  }
  ["example"]=>
  string(8) "document"
}
===DONE===

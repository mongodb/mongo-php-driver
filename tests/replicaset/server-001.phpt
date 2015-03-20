--TEST--
MongoDB\Driver\Server: Manager->getServer() returning correct server
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);


$doc = array("example" => "document");
$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert($doc);
$wresult = $manager->executeBulkWrite(NS, $bulk);

$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert($doc);

/* writes go to the primary */
$server = $wresult->getServer();

var_dump(
    $server->getHost(),
    $server->getTags(),
    $server->getLatency(),
    $server->getPort(),
    $server->getType() == MongoDB\Driver\Server::TYPE_RS_PRIMARY,
    $server->isPrimary(),
    $server->isSecondary(),
    $server->isArbiter(),
    $server->isHidden(),
    $server->isPassive()
);
$info = $server->getInfo(); // isMaster output changes between mongod versions
var_dump($info["setName"], $info["hosts"]);
var_dump($info["me"] == $server->getHost() . ":" . $server->getPort());
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
string(14) "192.168.112.10"
array(2) {
  ["dc"]=>
  string(2) "pa"
  ["ordinal"]=>
  string(3) "one"
}
int(%d)
int(3000)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
string(10) "REPLICASET"
array(2) {
  [0]=>
  string(19) "192.168.112.10:3000"
  [1]=>
  string(19) "192.168.112.10:3001"
}
bool(true)
===DONE===

--TEST--
MongoDB\Driver\Server: Manager->getServer() returning correct server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_enough_data_nodes(2, 2); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);


$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY);
$result = $manager->executeQuery(NS, new MongoDB\Driver\Query(array()), $rp);

/* writes go to the primary */
$server = $result->getServer();

var_dump(
    $server->getHost(),
    $server->getTags(),
    $server->getLatency(),
    $server->getPort(),
    $server->getType() == MongoDB\Driver\Server::TYPE_RS_SECONDARY,
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
string(%d) "%s"
array(2) {
  ["dc"]=>
  string(3) "nyc"
  ["ordinal"]=>
  string(3) "two"
}
int(%d)
int(%d)
bool(true)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
string(%s) "REPLICASET%S"
array(2) {
  [0]=>
  string(%d) "%s:%d"
  [1]=>
  string(%d) "%s:%d"
}
bool(true)
===DONE===

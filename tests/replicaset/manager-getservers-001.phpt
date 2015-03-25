--TEST--
MongoDB\Driver\Manager: getServers()
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);


$doc = array("example" => "document");
$bulk = new \MongoDB\Driver\BulkWrite();
$bulk->insert($doc);
$wresult = $manager->executeBulkWrite(NS, $bulk);


var_dump($manager->getServers());
$servers = $manager->getServers();

foreach($servers as $server) {
    printf("%s:%d - primary: %d, secondary: %d, arbiter: %d\n", $server->getHost(), $server->getPort(), $server->isPrimary(), $server->isSecondary(), $server->isArbiter());
}
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(3) {
  [0]=>
  object(MongoDB\Driver\Server)#5 (0) {
  }
  [1]=>
  object(MongoDB\Driver\Server)#6 (0) {
  }
  [2]=>
  object(MongoDB\Driver\Server)#7 (0) {
  }
}
192.168.112.10:3000 - primary: 1, secondary: 0, arbiter: 0
192.168.112.10:3001 - primary: 0, secondary: 1, arbiter: 0
192.168.112.10:3002 - primary: 0, secondary: 0, arbiter: 1
===DONE===

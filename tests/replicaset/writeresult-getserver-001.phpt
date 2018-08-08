--TEST--
MongoDB\Driver\Server: Manager->getServer() returning correct server
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_no_secondary(); ?>
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
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
int(%d)
int(%d)
===DONE===

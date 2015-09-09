--TEST--
MongoDB\Driver\Server::executeQuery() with conflicting read preference for secondary
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; NEEDS("REPLICASET"); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY);
$secondary = $manager->selectServer($rp);

echo throws(function() use ($secondary) {
    $rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY);
    $secondary->executeQuery(NS, new MongoDB\Driver\Query(array("x" => 1)), $rp);
}, "MongoDB\Driver\Exception\RuntimeException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\RuntimeException
not master and slaveOk=false
===DONE===

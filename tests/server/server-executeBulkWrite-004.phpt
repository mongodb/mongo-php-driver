--TEST--
MongoDB\Driver\Server::executeBulkWrite() with legacy write concern (replica set secondary)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_replica_set(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);
$server = $manager->selectServer(new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_SECONDARY));

$writeConcerns = array(1, 2, MongoDB\Driver\WriteConcern::MAJORITY);

foreach ($writeConcerns as $wc) {
    $bulk = new MongoDB\Driver\BulkWrite();
    $bulk->insert(array('wc' => $wc));

    echo throws(function() use ($server, $bulk, $wc) {
        $server->executeBulkWrite(NS, $bulk, new MongoDB\Driver\WriteConcern($wc));
    }, "MongoDB\Driver\Exception\RuntimeException"), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
OK: Got MongoDB\Driver\Exception\RuntimeException
not master
===DONE===

--TEST--
MongoDB\Driver\Server::executeQuery(): wrong options and values
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php NEEDS('REPLICASET'); CLEANUP(REPLICASET); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(REPLICASET);
$primary = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY);
$server = $manager->selectServer($primary);

$query = new MongoDB\Driver\Query(['x' => 3], ['projection' => ['y' => 1]]);

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['readPreference' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['readPreference' => new stdClass]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['unknown' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() use ($manager, $query) {
    $manager->executeQuery(NS, $query, ['writeConcern' => 'foo']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 'readPreference' option to be 'MongoDB\Driver\ReadPreference', string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 'readPreference' option to be 'MongoDB\Driver\ReadPreference', object given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'unknown'
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Unknown option 'writeConcern'
===DONE===

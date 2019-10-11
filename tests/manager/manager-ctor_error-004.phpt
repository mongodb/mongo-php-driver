--TEST--
MongoDB\Driver\Manager::__construct(): empty replicaSet argument
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function () {
    $manager = new MongoDB\Driver\Manager('mongodb://localhost:27017/?replicaSet=');
    $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function () {
    $manager = new MongoDB\Driver\Manager('mongodb://localhost:27017', ['replicaSet' => '']);
    $manager->executeQuery(NS, new MongoDB\Driver\Query([]));
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?replicaSet='. Value for URI option "replicaset" cannot be empty string.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Value for URI option "replicaSet" cannot be empty string.
===DONE===

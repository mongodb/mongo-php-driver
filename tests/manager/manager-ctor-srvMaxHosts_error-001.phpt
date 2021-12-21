--TEST--
MongoDB\Driver\Manager::__construct(): srvMaxHosts option validation
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/?srvMaxHosts=1');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017', ['srvMaxHosts' => 1]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb+srv://a.example.com/?srvMaxHosts=1&replicaSet=foo');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb+srv://a.example.com', ['srvMaxHosts' => 1, 'replicaSet' => 'foo']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb+srv://a.example.com/?srvMaxHosts=1&loadBalanced=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb+srv://a.example.com', ['srvMaxHosts' => 1, 'loadBalanced' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?srvMaxHosts=1'. srvmaxhosts must not be specified with a non-SRV URI.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: srvmaxhosts must not be specified with a non-SRV URI
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb+srv://a.example.com/?srvMaxHosts=1&replicaSet=foo'. srvmaxhosts must not be specified with replicaset.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: srvmaxhosts must not be specified with replicaset
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb+srv://a.example.com/?srvMaxHosts=1&loadBalanced=true'. srvmaxhosts must not be specified with loadbalanced=true.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: srvmaxhosts must not be specified with loadbalanced=true
===DONE===

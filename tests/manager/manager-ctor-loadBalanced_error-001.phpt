--TEST--
MongoDB\Driver\Manager::__construct(): loadBalanced option validation
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    create_test_manager('mongodb://localhost:27017,localhost:27018/?loadBalanced=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017,localhost:27018', ['loadBalanced' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/?loadBalanced=true&replicaSet=foo');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017', ['loadBalanced' => true, 'replicaSet' => 'foo']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/?loadBalanced=true&directConnection=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017', ['loadBalanced' => true, 'directConnection' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017,localhost:27018/?loadBalanced=true'. URI with "loadbalanced" enabled must not contain more than one host.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: URI with "loadbalanced" enabled must not contain more than one host
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?loadBalanced=true&replicaSet=foo'. URI with "loadbalanced" enabled must not contain option "replicaset".
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: URI with "loadbalanced" enabled must not contain option "replicaset"
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?loadBalanced=true&directConnection=true'. URI with "loadbalanced" enabled must not contain option "directconnection" enabled.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: URI with "loadbalanced" enabled must not contain option "directconnection" enabled
===DONE===

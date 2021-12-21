--TEST--
MongoDB\Driver\Manager::__construct(): srvServiceName option validation
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/?srvServiceName=foo');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017', ['srvServiceName' => 'foo']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?srvServiceName=foo'. srvservicename must not be specified with a non-SRV URI.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: srvservicename must not be specified with a non-SRV URI
===DONE===

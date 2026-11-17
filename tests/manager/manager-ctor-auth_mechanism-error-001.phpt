--TEST--
MongoDB\Driver\Manager::__construct(): GSSAPI authentication options are validated
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

// GSSAPI requires a username
echo throws(function() {
    create_test_manager('mongodb://localhost:27017/?authMechanism=GSSAPI');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['authMechanism' => 'GSSAPI']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// GSSAPI requires $external auth source
echo throws(function() {
    create_test_manager('mongodb://user@localhost:27017/?authMechanism=GSSAPI&authSource=admin');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['authMechanism' => 'GSSAPI', 'authSource' => 'admin', 'username' => 'user']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'GSSAPI' authentication mechanism requires a username.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'GSSAPI' authentication mechanism requires a username
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'GSSAPI' authentication mechanism requires "$external" authSource.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'GSSAPI' authentication mechanism requires "$external" authSource
===DONE===

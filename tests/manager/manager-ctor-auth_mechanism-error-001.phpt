--TEST--
MongoDB\Driver\Manager::__construct(): authentication options are validated
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/?authMechanism=GSSAPI&authSource=admin');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['authMechanism' => 'GSSAPI', 'authSource' => 'admin']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/?authMechanism=MONGODB-X509&authSource=admin');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['authMechanism' => 'MONGODB-X509', 'authSource' => 'admin']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://@localhost:27017/?authMechanism=SCRAM-SHA-1');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['username' => '', 'authMechanism' => 'SCRAM-SHA-1']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['password' => 'password', 'authMechanism' => 'MONGODB-X509']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?authMechanism=GSSAPI&authSource=admin'. 'GSSAPI' authentication mechanism requires a username.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'GSSAPI' authentication mechanism requires a username
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?authMechanism=MONGODB-X509&authSource=admin'. 'MONGODB-X509' authentication mechanism requires "$external" authSource, but "admin" was specified.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'MONGODB-X509' authentication mechanism requires "$external" authSource, but "admin" was specified
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://@localhost:27017/?authMechanism=SCRAM-SHA-1'. 'SCRAM-SHA-1' authentication mechanism requires a username.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'SCRAM-SHA-1' authentication mechanism requires a username
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'MONGODB-X509' authentication mechanism does not accept a password
===DONE===

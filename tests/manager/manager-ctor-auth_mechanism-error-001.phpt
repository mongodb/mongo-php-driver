--TEST--
MongoDB\Driver\Manager::__construct(): authentication options are validated
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/?authMechanism=GSSAPI&authSource=admin');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/', ['authMechanism' => 'GSSAPI', 'authSource' => 'admin']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/?authMechanism=MONGODB-X509&authSource=admin');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/', ['authMechanism' => 'MONGODB-X509', 'authSource' => 'admin']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://@localhost:27017/?authMechanism=SCRAM-SHA-1');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/', ['username' => '', 'authMechanism' => 'SCRAM-SHA-1']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/', ['password' => 'password', 'authMechanism' => 'MONGODB-X509']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?authMechanism=GSSAPI&authSource=admin'. GSSAPI and X509 require "$external" authSource.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: GSSAPI and X509 require "$external" authSource.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?authMechanism=MONGODB-X509&authSource=admin'. GSSAPI and X509 require "$external" authSource.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: GSSAPI and X509 require "$external" authSource.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://@localhost:27017/?authMechanism=SCRAM-SHA-1'. 'SCRAM-SHA-1' authentication mechanism requires username.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'SCRAM-SHA-1' authentication mechanism requires username.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: X509 authentication mechanism does not accept a password.
===DONE===

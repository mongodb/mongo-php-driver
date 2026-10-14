--TEST--
MongoDB\Driver\Manager::__construct(): SCRAM-SHA-1 authentication options are validated
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

// SCRAM-SHA-1 requires a username
echo throws(function() {
    create_test_manager('mongodb://@localhost:27017/?authMechanism=SCRAM-SHA-1');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['username' => '', 'authMechanism' => 'SCRAM-SHA-1']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// SCRAM-SHA-1 requires password
echo throws(function() {
    create_test_manager('mongodb://user@localhost:27017/?authMechanism=SCRAM-SHA-1');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['username' => 'user', 'authMechanism' => 'SCRAM-SHA-1']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://@localhost:27017/?authMechanism=SCRAM-SHA-1'. 'SCRAM-SHA-1' authentication mechanism requires a username.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'SCRAM-SHA-1' authentication mechanism requires a username
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://user@localhost:27017/?authMechanism=SCRAM-SHA-1'. 'SCRAM-SHA-1' authentication mechanism requires a password.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'SCRAM-SHA-1' authentication mechanism requires a password
===DONE===

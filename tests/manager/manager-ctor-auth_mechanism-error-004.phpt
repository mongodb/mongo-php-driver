--TEST--
MongoDB\Driver\Manager::__construct(): MONGODB-AWS authentication options are validated
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

// MONGODB-AWS requires a username
echo throws(function() {
    create_test_manager('mongodb://@localhost:27017/?authMechanism=MONGODB-AWS');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['username' => '', 'authMechanism' => 'MONGODB-AWS']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

// MONGODB-AWS requires a password
echo throws(function() {
    create_test_manager('mongodb://user@localhost:27017/?authMechanism=MONGODB-AWS');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
echo throws(function() {
    create_test_manager('mongodb://localhost:27017/', ['username' => 'user', 'authMechanism' => 'MONGODB-AWS']);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://@localhost:27017/?authMechanism=MONGODB-AWS'. 'MONGODB-AWS' authentication mechanism requires a non-empty username.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'MONGODB-AWS' authentication mechanism requires a non-empty username
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://user@localhost:27017/?authMechanism=MONGODB-AWS'. 'MONGODB-AWS' authentication mechanism does not accept a username or a password without the other.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: 'MONGODB-AWS' authentication mechanism does not accept a username or a password without the other
===DONE===

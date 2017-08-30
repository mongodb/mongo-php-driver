--TEST--
MongoDB\Driver\Manager::__construct(): invalid types in URI options arrays
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* Note: generic boolean options (e.g. "ssl") are not tested because the driver
 * uses bson_iter_as_bool() to cast the value to a boolean for assignment.
 *
 * Read concern, read preference, and write concern options are tested in their
 * respective test files. */

echo "Testing 32-bit integer options:\n";

$integerOptions = [
    'connectTimeoutMS',
    'heartbeatFrequencyMS',
    'localThresholdMS',
    'serverSelectionTimeoutMS',
    'socketCheckIntervalMS',
    'socketTimeoutMS',
];

$invalidIntegerValues = [
    true,
    1.0,
    'string',
    new MongoDB\BSON\ObjectId,
    [ 1, 2, 3 ],
    ['x' => 1],
];

foreach ($integerOptions as $option) {
    foreach ($invalidIntegerValues as $value) {
        echo throws(function() use ($option, $value) {
            new MongoDB\Driver\Manager(null, [$option => $value]);
        }, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
    }
}

echo "\nTesting string options:\n";

$stringOptions = [
    'appname',
    'authMechanism',
    'authSource',
    'gssapiServiceName',
    'password',
    'replicaSet',
    'username',
];

$invalidStringValues = [
    true,
    1.0,
    42,
    new MongoDB\BSON\ObjectId,
    [ 1, 2, 3 ],
    ['x' => 1],
];

foreach ($stringOptions as $option) {
    foreach ($invalidStringValues as $value) {
        echo throws(function() use ($option, $value) {
            new MongoDB\Driver\Manager(null, [$option => $value]);
        }, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
    }
}

echo "\nTesting document options:\n";

$invalidDocumentValues = [
    true,
    1.0,
    42,
    'string',
    new MongoDB\BSON\ObjectId,
    [ 1, 2, 3 ],
];

foreach ($invalidDocumentValues as $value) {
    echo throws(function() use ($value) {
        new MongoDB\Driver\Manager(null, ['authMechanismProperties' => $value]);
    }, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing 32-bit integer options:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "connectTimeoutMS" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "connectTimeoutMS" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "connectTimeoutMS" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "connectTimeoutMS" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "connectTimeoutMS" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "connectTimeoutMS" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "heartbeatFrequencyMS" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "heartbeatFrequencyMS" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "heartbeatFrequencyMS" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "heartbeatFrequencyMS" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "heartbeatFrequencyMS" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "heartbeatFrequencyMS" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "localThresholdMS" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "localThresholdMS" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "localThresholdMS" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "localThresholdMS" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "localThresholdMS" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "localThresholdMS" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "serverSelectionTimeoutMS" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "serverSelectionTimeoutMS" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "serverSelectionTimeoutMS" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "serverSelectionTimeoutMS" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "serverSelectionTimeoutMS" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "serverSelectionTimeoutMS" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketCheckIntervalMS" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketCheckIntervalMS" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketCheckIntervalMS" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketCheckIntervalMS" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketCheckIntervalMS" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketCheckIntervalMS" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketTimeoutMS" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketTimeoutMS" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketTimeoutMS" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketTimeoutMS" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketTimeoutMS" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "socketTimeoutMS" URI option, document given

Testing string options:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "appname" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "appname" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "appname" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "appname" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "appname" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "appname" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authMechanism" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authMechanism" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authMechanism" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authMechanism" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authMechanism" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authMechanism" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authSource" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authSource" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authSource" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authSource" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authSource" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "authSource" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "gssapiServiceName" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "gssapiServiceName" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "gssapiServiceName" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "gssapiServiceName" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "gssapiServiceName" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "gssapiServiceName" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "password" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "password" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "password" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "password" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "password" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "password" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "replicaSet" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "replicaSet" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "replicaSet" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "replicaSet" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "replicaSet" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "replicaSet" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "username" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "username" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "username" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "username" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "username" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "username" URI option, document given

Testing document options:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected array or object for "authMechanismProperties" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected array or object for "authMechanismProperties" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected array or object for "authMechanismProperties" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected array or object for "authMechanismProperties" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected array or object for "authMechanismProperties" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected array or object for "authMechanismProperties" URI option, array given
===DONE===

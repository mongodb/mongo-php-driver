--TEST--
MongoDB\Driver\Manager::__construct(): invalid types in URI options arrays
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Note: read concern, read preference, and write concern options are tested in
 * their respective test files. */

echo "Testing boolean options:\n";

$booleanOptions = [
    'directConnection',
    'journal',
    'loadBalanced',
    'retryReads',
    'retryWrites',
    'serverSelectionTryOnce',
    'tls',
    'tlsAllowInvalidCertificates',
    'tlsAllowInvalidHostnames',
    'tlsDisableCertificateRevocationCheck',
    'tlsDisableOCSPEndpointCheck',
    'tlsInsecure',
];

$invalidBooleanValues = [
    1.0,
    42,
    'string',
    new MongoDB\BSON\ObjectId,
    [ 1, 2, 3 ],
    ['x' => 1],
];

foreach ($booleanOptions as $option) {
    foreach ($invalidBooleanValues as $value) {
        echo throws(function() use ($option, $value) {
            create_test_manager(null, [$option => $value]);
        }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
    }
}

echo "\nTesting 32-bit integer options:\n";

$integerOptions = [
    'connectTimeoutMS',
    'heartbeatFrequencyMS',
    'localThresholdMS',
    'serverSelectionTimeoutMS',
    'socketCheckIntervalMS',
    'socketTimeoutMS',
    'srvMaxHosts',
    'zlibCompressionLevel',
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
            create_test_manager(null, [$option => $value]);
        }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
    }
}

echo "\nTesting string options:\n";

$stringOptions = [
    'appname',
    'authMechanism',
    'authSource',
    'compressors',
    'password',
    'replicaSet',
    'srvServiceName',
    'tlsCAFile',
    'tlsCertificateKeyFile',
    'tlsCertificateKeyFilePassword',
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
            create_test_manager(null, [$option => $value]);
        }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
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
        create_test_manager(null, ['authMechanismProperties' => $value]);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Testing boolean options:
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "directConnection" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "directConnection" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "directConnection" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "directConnection" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "directConnection" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "directConnection" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "journal" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "journal" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "journal" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "journal" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "journal" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "journal" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "loadBalanced" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "loadBalanced" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "loadBalanced" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "loadBalanced" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "loadBalanced" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "loadBalanced" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryReads" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryReads" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryReads" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryReads" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryReads" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryReads" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryWrites" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryWrites" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryWrites" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryWrites" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryWrites" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "retryWrites" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "serverSelectionTryOnce" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "serverSelectionTryOnce" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "serverSelectionTryOnce" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "serverSelectionTryOnce" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "serverSelectionTryOnce" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "serverSelectionTryOnce" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tls" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tls" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tls" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tls" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tls" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tls" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidCertificates" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidCertificates" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidCertificates" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidCertificates" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidCertificates" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidCertificates" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidHostnames" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidHostnames" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidHostnames" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidHostnames" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidHostnames" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsAllowInvalidHostnames" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableCertificateRevocationCheck" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableCertificateRevocationCheck" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableCertificateRevocationCheck" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableCertificateRevocationCheck" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableCertificateRevocationCheck" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableCertificateRevocationCheck" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableOCSPEndpointCheck" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableOCSPEndpointCheck" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableOCSPEndpointCheck" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableOCSPEndpointCheck" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableOCSPEndpointCheck" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsDisableOCSPEndpointCheck" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsInsecure" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsInsecure" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsInsecure" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsInsecure" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsInsecure" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected boolean for "tlsInsecure" URI option, document given

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
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "srvMaxHosts" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "srvMaxHosts" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "srvMaxHosts" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "srvMaxHosts" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "srvMaxHosts" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "srvMaxHosts" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "zlibCompressionLevel" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "zlibCompressionLevel" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "zlibCompressionLevel" URI option, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "zlibCompressionLevel" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "zlibCompressionLevel" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected 32-bit integer for "zlibCompressionLevel" URI option, document given

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
Expected string for "compressors" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "compressors" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "compressors" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "compressors" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "compressors" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "compressors" URI option, document given
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
Expected string for "srvServiceName" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "srvServiceName" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "srvServiceName" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "srvServiceName" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "srvServiceName" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "srvServiceName" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCAFile" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCAFile" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCAFile" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCAFile" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCAFile" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCAFile" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFile" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFile" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFile" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFile" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFile" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFile" URI option, document given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFilePassword" URI option, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFilePassword" URI option, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFilePassword" URI option, 32-bit integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFilePassword" URI option, ObjectId given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFilePassword" URI option, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected string for "tlsCertificateKeyFilePassword" URI option, document given
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

--TEST--
MongoDB\Driver\Manager::__construct(): tlsAllowInvalidCertificates cannot be combined with tlsDisableCertificateRevocationCheck
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/?tlsAllowInvalidCertificates=true&tlsDisableCertificateRevocationCheck=true');
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/', ['tlsAllowInvalidCertificates' => true, 'tlsDisableCertificateRevocationCheck' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/?tlsAllowInvalidCertificates=true', ['tlsDisableCertificateRevocationCheck' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

echo throws(function() {
    new MongoDB\Driver\Manager('mongodb://localhost:27017/?tlsDisableCertificateRevocationCheck=true', ['tlsAllowInvalidCertificates' => true]);
}, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=true&tlsDisableCertificateRevocationCheck=true'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be combined with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be combined with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be combined with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
===DONE===

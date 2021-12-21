--TEST--
MongoDB\Driver\Manager::__construct(): Test invalid URI option combinations
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$invalidCombinations = [
    ['tlsInsecure', 'tlsAllowInvalidHostnames'],
    ['tlsInsecure', 'tlsAllowInvalidCertificates'],
    ['tlsInsecure', 'tlsDisableOCSPEndpointCheck'],
    ['tlsInsecure', 'tlsDisableCertificateRevocationCheck'],
    ['tlsAllowInvalidCertificates', 'tlsDisableOCSPEndpointCheck'],
    ['tlsAllowInvalidCertificates', 'tlsDisableCertificateRevocationCheck'],
];

foreach ($invalidCombinations as list($optionA, $optionB)) {
    foreach ([false, true] as $valueA) {
        foreach ([false, true] as $valueB) {
            echo throws(function() use ($optionA, $valueA, $optionB, $valueB) {
                create_test_manager(sprintf(
                    'mongodb://localhost:27017/?%s=%s&%s=%s',
                    $optionA,
                    $valueA ? 'true' : 'false',
                    $optionB,
                    $valueB ? 'true' : 'false'
                ));
            }, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";

            echo throws(function() use ($optionA, $valueA, $optionB, $valueB) {
                create_test_manager(
                    sprintf(
                        'mongodb://localhost:27017/?%s=%s',
                        $optionA,
                        $valueA ? 'true' : 'false'
                    ),
                    [ $optionB => $valueB ]
                );
            }, "MongoDB\Driver\Exception\InvalidArgumentException"), "\n";
        }
    }

    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsAllowInvalidHostnames=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsAllowInvalidHostnames=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsAllowInvalidHostnames=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsAllowInvalidHostnames=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsAllowInvalidCertificates=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsAllowInvalidCertificates=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsAllowInvalidCertificates=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsAllowInvalidCertificates=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsDisableOCSPEndpointCheck=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsDisableOCSPEndpointCheck=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsDisableOCSPEndpointCheck=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsDisableOCSPEndpointCheck=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsDisableCertificateRevocationCheck=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=false&tlsDisableCertificateRevocationCheck=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsDisableCertificateRevocationCheck=false'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsInsecure=true&tlsDisableCertificateRevocationCheck=true'. tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=false&tlsDisableOCSPEndpointCheck=false'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=false&tlsDisableOCSPEndpointCheck=true'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=true&tlsDisableOCSPEndpointCheck=false'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=true&tlsDisableOCSPEndpointCheck=true'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=false&tlsDisableCertificateRevocationCheck=false'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=false&tlsDisableCertificateRevocationCheck=true'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=true&tlsDisableCertificateRevocationCheck=false'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: 'mongodb://localhost:27017/?tlsAllowInvalidCertificates=true&tlsDisableCertificateRevocationCheck=true'. tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck

===DONE===

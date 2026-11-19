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
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsinsecure may not be specified with tlsallowinvalidcertificates, tlsallowinvalidhostnames, tlsdisableocspendpointcheck, or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck

OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse MongoDB URI: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Failed to parse URI options: tlsallowinvalidcertificates may not be specified with tlsdisableocspendpointcheck or tlsdisablecertificaterevocationcheck

===DONE===

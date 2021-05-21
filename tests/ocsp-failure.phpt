--TEST--
Connection with OCSP checks fails
--SKIPIF--
<?php
if (getenv('TESTS') !== 'tests/ocsp-failure.phpt') { echo "skip OCSP tests not wanted\n"; }
?>
--FILE--
<?php
require_once __DIR__ . "/utils/basic.inc";

$ping = new \MongoDB\Driver\Command(['ping' => 1]);

// Expect command to fail with the provided options
$m = create_test_manager();
echo throws (function () use ($m, $ping) {
    $m->executeCommand('admin', $ping);
}, "MongoDB\Driver\Exception\ConnectionTimeoutException"), "\n";

// Always expect command to pass when using insecure option
$m = create_test_manager(URI, ['tlsInsecure' => true]);
$m->executeCommand('admin', $ping);

// Always expect command to pass when allowing invalid certificates
$m = create_test_manager(URI, ['tlsAllowInvalidCertificates' => true]);
$m->executeCommand('admin', $ping);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionTimeoutException
No suitable servers found (`serverSelectionTryOnce` set): [%s calling %s on '%s:%d']
===DONE===

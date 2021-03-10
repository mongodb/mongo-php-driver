--TEST--
Connection with OCSP checks successful
--SKIPIF--
<?php
if (getenv('TESTS') !== 'tests/ocsp-success.phpt') { echo "skip OCSP tests not wanted\n"; }
?>
--FILE--
<?php
require_once __DIR__ . "/utils/basic.inc";

$ping = new \MongoDB\Driver\Command(['ping' => 1]);

// Expect command to pass with the provided options
$m = create_test_manager();
$m->executeCommand('admin', $ping);

// Always expect command to pass when using insecure option
$m = create_test_manager(URI, ['tlsInsecure' => true]);
$m->executeCommand('admin', $ping);

// Always expect command to pass when allowing invalid certificates
$m = create_test_manager(URI, ['tlsAllowInvalidCertificates' => true]);
$m->executeCommand('admin', $ping);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===

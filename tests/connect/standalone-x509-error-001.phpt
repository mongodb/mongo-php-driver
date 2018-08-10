--TEST--
X509 connection should not reuse previous stream after an auth failure
--XFAIL--
parse_url() tests must be reimplemented (PHPC-1177)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(); ?>
<?php skip_if_not_ssl(); ?>
<?php skip_if_not_auth_mechanism('MONGODB-X509'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$SSL_DIR = realpath(__DIR__ . '/../../scripts/ssl/');

$driverOptions = [
    // libmongoc does not allow the hostname to be overridden as "server"
    'allow_invalid_hostname' => true,
    'ca_file' => $SSL_DIR . '/ca.pem',
    'pem_file' => $SSL_DIR . '/client.pem',
];

// Wrong username for X509 authentication
$parsed = parse_url(URI);
$dsn = sprintf('mongodb://username@%s:%d/?ssl=true&authMechanism=MONGODB-X509', $parsed['host'], $parsed['port']);

// Both should fail with auth failure, without reusing the previous stream
for ($i = 0; $i < 2; $i++) {
    echo throws(function() use ($dsn, $driverOptions) {
        $manager = new MongoDB\Driver\Manager($dsn, [], $driverOptions);
        $cursor = $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));
        var_dump($cursor->toArray()[0]);
    }, 'MongoDB\Driver\Exception\AuthenticationException', 'executeCommand'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\AuthenticationException thrown from executeCommand
auth failed
OK: Got MongoDB\Driver\Exception\AuthenticationException thrown from executeCommand
auth failed
===DONE===
